//
// Created by Johannes on 02.06.2018.
//

#include <iostream>
#include <QtWidgets/QInputDialog>
#include "MeshWrapper.h"

MeshWrapper::MeshWrapper() {
}


void MeshWrapper::loadMesh(const char *path) {

    if(path != nullptr) {
        if (!OpenMesh::IO::read_mesh(mesh, path, opt)) {
            std::cerr << "Error loading mesh from file " << path << std::endl;
        }
    }else{
        mesh.clear();
    }
}

void MeshWrapper::writeMesh(const char *path){
    if (!OpenMesh::IO::write_mesh(mesh, path))    {
        std::cerr << "Error writing mesh to file " << path << std::endl;
    }
}

//gets vertices in triangle order, if mesh should not be triangulated, an index array must be implemented with triangulated indices
void MeshWrapper::getVerticesAndNormalsTriangulated(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals) {

    mesh.request_face_normals();
    mesh.update_normals();
    mesh.request_vertex_normals();

    HE_MESH meshTriangulated = HE_MESH(mesh);
    //triangulation needed for DRAW_TRIANGLES
    meshTriangulated.triangulate();

    meshTriangulated.request_face_normals();
    meshTriangulated.update_normals();
    meshTriangulated.request_vertex_normals();

    OpenMesh::Vec3f v, n;

    //run throught each face(witch is a triangle)
    for (HE_MESH::FaceIter f_it = meshTriangulated.faces_begin(); f_it != meshTriangulated.faces_end(); ++f_it) {
        //run throught each vertex in this face
        for (HE_MESH::FaceVertexIter fv_it = meshTriangulated.fv_begin(*f_it); fv_it != meshTriangulated.fv_end(*f_it); ++fv_it) {
            v = meshTriangulated.point(*fv_it);
            vertices.push_back(glm::vec3(v[0], v[1], v[2]));
            n = meshTriangulated.normal(*fv_it);
            normals.push_back(glm::vec3(n[0], n[1], n[2]));

        }
    }
}

void MeshWrapper::getVertices(std::vector<glm::vec3> &vertices) {

    for (HE_MESH::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        OpenMesh::Vec3f v = mesh.point(*v_it);
        vertices.push_back(glm::vec3(v[0], v[1], v[2]));
    }

}

void MeshWrapper::getLineVertices(std::vector<glm::vec3> &vertices, std::vector<bool> &selected) {


    for(HE_MESH::EdgeIter e_it = mesh.edges_begin(); e_it != mesh.edges_end(); ++e_it){
        HE_MESH::VertexHandle vh1 = mesh.to_vertex_handle(mesh.halfedge_handle(*e_it, 0));
        HE_MESH::VertexHandle vh2 = mesh.from_vertex_handle(mesh.halfedge_handle(*e_it, 0));

        OpenMesh::Vec3f v1 = mesh.point(vh1);
        OpenMesh::Vec3f v2 = mesh.point(vh2);

        vertices.push_back(glm::vec3(v1[0], v1[1], v1[2]));
        vertices.push_back(glm::vec3(v2[0], v2[1], v2[2]));

        if(checkSelectedHalfEdges(vh1, vh2)){
            selected.push_back(true);
            selected.push_back(true);
        }else{
            selected.push_back(false);
            selected.push_back(false);
        }

    }
}

bool MeshWrapper::checkSelectedHalfEdges(HE_MESH::VertexHandle v1, HE_MESH::VertexHandle v2){
    for(HE_MESH::HalfedgeHandle heh : selectedHalfEdges){
        std::pair<HE_MESH::VertexHandle, HE_MESH::VertexHandle> vertices = getVerticesFromHalfEdge(heh);
        if((vertices.first == v1 && vertices.second == v2) || (vertices.first == v2 && vertices.second == v1)){
            return true;
        }
    }
    return false;
}



void MeshWrapper::moveVertex(HE_MESH::VertexHandle v_h, glm::vec3 relativeMovement) {
    OpenMesh::Vec3f newPoint =
            mesh.point(v_h) + HE_MESH::Point(relativeMovement.x, relativeMovement.y, relativeMovement.z);
    mesh.set_point(v_h, newPoint);

}

bool MeshWrapper::selectVertex(glm::vec3 pos, float radius) {
    bool isSelected = false;
    OpenMesh::Vec3f vertexPos;
    vertexPos[0] = pos.x;
    vertexPos[1] = pos.y;
    vertexPos[2] = pos.z;


    for (HE_MESH::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        OpenMesh::Vec3f meshPoint = mesh.point(*v_it);
        float xDif = meshPoint[0] - vertexPos[0];
        float yDif = meshPoint[1] - vertexPos[1];
        float zDif = meshPoint[2] - vertexPos[2];


        if ((xDif < radius && xDif > -radius) && (yDif < radius && yDif > -radius) &&
            (zDif < radius && zDif > -radius)) {

            if (std::find(selectedVertices.begin(), selectedVertices.end(), v_it) != selectedVertices.end()) {
                //deselect
                selectedVertices.erase(std::find(selectedVertices.begin(), selectedVertices.end(), v_it));
                this->setVertexWeight(*v_it, 1.0);
            } else {
                //select
                selectedVertices.push_back(*v_it);
                this->setVertexWeight(*v_it, 2.0);
                isSelected = true;
            }
        }

    }

    selectedHalfEdges.clear();
    if(selectedVertices.size() >= 2){
        for(int i = 0; i < selectedVertices.size()-1; i++){
            selectHalfEdge(selectedVertices.at(i), selectedVertices.at(i + 1));
        }
    }
    return isSelected;
}

void MeshWrapper::selectHalfEdge(HE_MESH::VertexHandle v1, HE_MESH::VertexHandle v2){

    for(HE_MESH::VertexOHalfedgeIter voh_it = mesh.voh_iter(v1); voh_it.is_valid(); ++voh_it) {
        // Iterate over all outgoing halfedges...

        HE_MESH::VertexHandle vh = mesh.to_vertex_handle(*voh_it);

        if(vh == v2){
            selectedHalfEdges.push_back(*voh_it);
        }
    }
}

void MeshWrapper::clearSelectedEdges(){
    selectedHalfEdges.clear();
}

std::pair<HE_MESH::VertexHandle, HE_MESH::VertexHandle> MeshWrapper::getVerticesFromHalfEdge(HE_MESH::HalfedgeHandle heh){
    std::pair<HE_MESH::VertexHandle, HE_MESH::VertexHandle> vertices;
    vertices.first =  mesh.from_vertex_handle(heh);
    vertices.second =  mesh.to_vertex_handle(heh);
    return  vertices;
}



void MeshWrapper::deselectAll() {
    selectedVertices.clear();
    selectedHalfEdges.clear();
}


std::vector<glm::vec3> MeshWrapper::getSelectedVertices() {
    std::vector<glm::vec3> selected;

    for (HE_MESH::VertexHandle v_h : selectedVertices) {
        selected.push_back(glm::vec3(mesh.point(v_h)[0], mesh.point(v_h)[1], mesh.point(v_h)[2]));
    }
    return selected;
}

void MeshWrapper::moveSelectedVertices(glm::vec3 relativeMovement) {
    for (HE_MESH::VertexHandle v_h : selectedVertices) {
        moveVertex(v_h, relativeMovement);
    }

}

void MeshWrapper::deleteSelectedVertices() {
    mesh.request_face_status();
    mesh.request_edge_status();
    mesh.request_vertex_status();


    for (HE_MESH::VertexHandle v_h : selectedVertices) {
        mesh.delete_vertex(v_h, true);
    }
    mesh.garbage_collection();
    deselectAll();

}

void MeshWrapper::addVertex(glm::vec3 vertex) {
    OpenMesh::Vec3f const newPoint = HE_MESH::Point(vertex.x, vertex.y, vertex.z);
    mesh.new_vertex(newPoint);
}

void MeshWrapper::makeSelectedFace() {
    if (selectedVertices.size() == 3) {
        mesh.add_face(selectedVertices);
    }
}

void MeshWrapper::subdivision() {
    backstack.push_back(mesh);

    this->mesh = catmull(this->mesh, 1);
}

void MeshWrapper::undo() {
    if(backstack.size() > 0) {
        mesh = backstack.at(backstack.size() - 1);
        backstack.pop_back();
    }
}

void MeshWrapper::setVertexWeight(HE_MESH::VertexHandle vertexHandle, float weight) {
    this->mesh.property(this->mesh.vp_fourth, vertexHandle) = weight;
}

float MeshWrapper::getVertexWeight(HE_MESH::VertexHandle vertexHandle) {
    return this->mesh.property(this->mesh.vp_fourth, vertexHandle);
}






