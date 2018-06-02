//
// Created by Johannes on 02.06.2018.
//

#ifndef CG_ANW_MESHWRAPPER_H
#define CG_ANW_MESHWRAPPER_H

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <libs/glm-0.9.7.2/glm/vec3.hpp>


// Define my personal traits
struct Traits : OpenMesh::DefaultTraits
{
    // Let Point and Normal be a glm vec3
    typedef OpenMesh::Vec3f Point;
    typedef OpenMesh::Vec3f Normal;

};

// Define my mesh with the new traits!
typedef OpenMesh::PolyMesh_ArrayKernelT<Traits>  HE_MESH;


class MeshWrapper {
public:

    MeshWrapper();

    void loadMesh(const char *path);

    void moveVertex(HE_MESH::VertexHandle v_h, glm::vec3 relativeMovement);

    void moveSelectedVertices(glm::vec3 relativeMovement);

    void getVerticesAndNormalsTriangulated(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals);

    void getVertices(std::vector<glm::vec3> &vertices);

    void selectVertex(glm::vec3 pos);

    void deselectAll();

    std::vector<glm::vec3> getSelectedVertices();

    void deleteSelectedVertices();

    void addVertex(glm::vec3 vertex);


private:



    HE_MESH mesh;

    OpenMesh::IO::Options opt;

    std::vector<HE_MESH::VertexHandle> selectedVertices;


};


#endif //CG_ANW_MESHWRAPPER_H
