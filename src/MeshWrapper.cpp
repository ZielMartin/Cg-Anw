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

void MeshWrapper::getLineVertices(std::vector<glm::vec3> &vertices) {

    for (HE_MESH::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
        //run throught each vertex in this face
        HE_MESH::FaceVertexIter fv_it_first = mesh.fv_begin(*f_it);

        int counter = mesh.valence(*f_it);
        for (HE_MESH::FaceVertexIter fv_it = mesh.fv_begin(*f_it); fv_it != mesh.fv_end(*f_it); ++fv_it) {
            counter--;

            OpenMesh::Vec3f v = mesh.point(*fv_it);
            if(fv_it != fv_it_first){
                vertices.push_back(glm::vec3(v[0], v[1], v[2]));
            }
            vertices.push_back(glm::vec3(v[0], v[1], v[2]));

            if(counter == 0){
                v = mesh.point(*mesh.fv_begin(*f_it));
                vertices.push_back(glm::vec3(v[0], v[1], v[2]));
            }

        }
    }

}


void MeshWrapper::moveVertex(HE_MESH::VertexHandle v_h, glm::vec3 relativeMovement) {
    OpenMesh::Vec3f newPoint =
            mesh.point(v_h) + HE_MESH::Point(relativeMovement.x, relativeMovement.y, relativeMovement.z);
    mesh.set_point(v_h, newPoint);

}

void MeshWrapper::selectVertex(glm::vec3 pos, float radius) {
    OpenMesh::Vec3f vertexPos;
    vertexPos[0] = pos.x;
    vertexPos[1] = pos.y;
    vertexPos[2] = pos.z;


    for (HE_MESH::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        float xDif = mesh.point(*v_it)[0] - vertexPos[0];
        float yDif = mesh.point(*v_it)[1] - vertexPos[1];
        float zDif = mesh.point(*v_it)[2] - vertexPos[2];


        if ((xDif < radius && xDif > -radius) && (yDif < radius && yDif > -radius) &&
            (zDif < radius && zDif > -radius)) {

            if (std::find(selectedVertices.begin(), selectedVertices.end(), v_it) != selectedVertices.end()) {
                //deselect
                selectedVertices.erase(std::find(selectedVertices.begin(), selectedVertices.end(), v_it));
            } else {
                //select
                selectedVertices.push_back(*v_it);
            }
        }
    }
}

void MeshWrapper::deselectAll() {
    selectedVertices.clear();
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

    catmull.attach(mesh);
    catmull(1);
    catmull.detach();

}

void MeshWrapper::undo() {
    if(backstack.size() > 0) {
        mesh = backstack.at(backstack.size() - 1);
        backstack.pop_back();
    }
}






