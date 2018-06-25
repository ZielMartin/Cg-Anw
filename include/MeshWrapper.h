//
// Created by Johannes on 02.06.2018.
//

#ifndef CG_ANW_MESHWRAPPER_H
#define CG_ANW_MESHWRAPPER_H



#include "MyMesh.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/CatmullClarkT.hh>


#include <glm/vec3.hpp>


class MeshWrapper {
public:

    MeshWrapper();

    void loadMesh(const char *path);

    void moveVertex(HE_MESH::VertexHandle v_h, glm::vec3 relativeMovement);

    void moveSelectedVertices(glm::vec3 relativeMovement);

    void getVerticesAndNormalsTriangulated(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals);

    void getVertices(std::vector<glm::vec3> &vertices);

    void getLineVertices(std::vector<glm::vec3> &vertices, std::vector<bool> &selected);

    void selectVertex(glm::vec3 pos, float radius);

    void deleteSelectedVertices();

    void deselectAll();

    std::vector<glm::vec3> getSelectedVertices();

    void addVertex(glm::vec3 vertex);

    void makeSelectedFace();

    void subdivision();

    void undo();


private:



    HE_MESH mesh;

    std::vector<HE_MESH> backstack;

    OpenMesh::Subdivider::Uniform::CatmullClarkT<HE_MESH> catmull;

    OpenMesh::IO::Options opt;

    std::vector<HE_MESH::VertexHandle> selectedVertices;

    /*
     * selecting order determines which half-edge is selected
     */
    std::vector<HE_MESH::HalfedgeHandle> selectedHalfEdges;

    std::vector<HE_MESH::EdgeHandle> selectedEdges;


    std::pair<HE_MESH::VertexHandle, HE_MESH::VertexHandle> getVerticesFromHalfEdge(HE_MESH::HalfedgeHandle heh);

    void selectHalfEdge(HE_MESH::VertexHandle v1, HE_MESH::VertexHandle v2);

    bool checkSelectedHalfEdges(HE_MESH::VertexHandle v1, HE_MESH::VertexHandle v2);




};


#endif //CG_ANW_MESHWRAPPER_H
