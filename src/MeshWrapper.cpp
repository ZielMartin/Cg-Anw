//
// Created by Johannes on 02.06.2018.
//

#include <iostream>
#include "MeshWrapper.h"

MeshWrapper::MeshWrapper() {

}


void MeshWrapper::loadMesh(const char *path) {

    if (!OpenMesh::IO::read_mesh(mesh, path, opt)) {
        std::cerr << "Error loading mesh from file " << path << std::endl;
    }
    //triangulation needed for DRAW_TRIANGLES without index buffer
    mesh.triangulate();


    //if this is not here, normals are not calculate on first getVerticesAndNormals call?!Oo
    mesh.request_face_normals();
    mesh.update_normals();
    mesh.request_vertex_normals();
}

//gets vertices in triangle order, if mesh should not be triangulated, an index array must be implemented -
//and this function would not be necessary anymore
void MeshWrapper::getVerticesAndNormals(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals) {

    mesh.request_face_normals();
    mesh.update_normals();
    mesh.request_vertex_normals();

    OpenMesh::Vec3f v, n;

    //run throught each face(witch is a triangle)
    for (HE_MESH::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
        //run throught each vertex in this face
        for (HE_MESH::FaceVertexIter fv_it = mesh.fv_begin(*f_it); fv_it != mesh.fv_end(*f_it); ++fv_it) {
            v = mesh.point(*fv_it);
            vertices.push_back(glm::vec3(v[0],v[1],v[2]));
            n = mesh.normal(*fv_it);
            normals.push_back(glm::vec3(n[0],n[1],n[2]));

        }
    }
}


void MeshWrapper::moveVertex(HE_MESH::VertexIter v_it, glm::vec3 relativeMovement) {
    OpenMesh::Vec3f newPoint =  mesh.point(*v_it) + HE_MESH::Point(relativeMovement.x, relativeMovement.y, relativeMovement.z);
    mesh.set_point(*v_it, newPoint);
}

void MeshWrapper::selectVertex(glm::vec3 pos){
    OpenMesh::Vec3f vertexPos;
    vertexPos[0] = pos.x;
    vertexPos[1] = pos.y;
    vertexPos[2] = pos.z;


    float tolerance = 0.02;

    for(HE_MESH::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it){
        float xDif = mesh.point(*v_it)[0] - vertexPos[0];
        float yDif = mesh.point(*v_it)[1] - vertexPos[1];
        float zDif = mesh.point(*v_it)[2] - vertexPos[2];


        if((xDif < tolerance && xDif > -tolerance) && (yDif < tolerance && yDif > -tolerance) && (zDif < tolerance && zDif > -tolerance)){

            if(std::find(selectedVertices.begin(), selectedVertices.end(), v_it) != selectedVertices.end()){
                //deselect
                selectedVertices.erase(std::find(selectedVertices.begin(), selectedVertices.end(), v_it));
            }else{
                //select
                selectedVertices.push_back(v_it);
            }
        }
    }
}

void MeshWrapper::deselectAll() {
    selectedVertices.clear();
}


std::vector<glm::vec3> MeshWrapper::getSelectedVertices(){
    std::vector<glm::vec3> selected;

    for(HE_MESH::VertexIter v_it : selectedVertices){
        selected.push_back(glm::vec3(mesh.point(*v_it)[0], mesh.point(*v_it)[1], mesh.point(*v_it)[2]));
    }
    return selected;
}

void MeshWrapper::moveSelectedVertices(glm::vec3 relativeMovement){
    for(HE_MESH::VertexIter v_it : selectedVertices){
        moveVertex(v_it, relativeMovement);
    }

}

void MeshWrapper::deleteSelectedVertices(){
    mesh.request_face_status();
    mesh.request_edge_status();
    mesh.request_vertex_status();

    for(HE_MESH::VertexIter v_it : selectedVertices){
        mesh.delete_vertex(*v_it, true);
    }
    mesh.garbage_collection();
    deselectAll();

}





