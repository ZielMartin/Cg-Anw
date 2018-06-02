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
}

//gets vertices in triangle order, if mesh should not be triangulated, an index array must be implemented -
//and this function would not be necessary anymore
std::vector<glm::vec3> MeshWrapper::getVertices() {
    std::vector<glm::vec3> vertices;
    OpenMesh::Vec3f v;

    //run throught each face(witch is a triangle)
    for (HE_MESH::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
        //run throught each vertex in this face
        for (HE_MESH::FaceVertexIter fv_it = mesh.fv_begin(*f_it); fv_it != mesh.fv_end(*f_it); ++fv_it) {
            v = mesh.point(*fv_it);
            vertices.push_back(glm::vec3(v[0],v[1],v[2]));

        }
    }
    return vertices;
}


void MeshWrapper::moveVertex(HE_MESH::VertexIter v_it, glm::vec3 relativeMovement) {
    mesh.set_point(*v_it, mesh.point(*v_it) + HE_MESH::Point(relativeMovement.x, relativeMovement.y, relativeMovement.z));
}



//right now, normals are calculated in Renderer
void MeshWrapper::recalcVertexNormals() {

    // we need face normals to update the vertex normals
    mesh.request_face_normals();

    // let the mesh update the normals
    mesh.update_normals();
}


