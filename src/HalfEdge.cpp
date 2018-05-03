//
// Created by ZielM on 30.04.2018.
//

#include "HalfEdge.h"
#include <algorithm>

namespace cg {
    void HE_Wrapper::clear() {
        verts.clear();
        edges.clear();
        faces.clear();
    }

    void HE_Wrapper::deleteVert(VertPointer deleteMe) {

    }

    void HE_Wrapper::deleteEdge(EdgePointer deleteMe) {

    }

    void HE_Wrapper::deleteFace(FacePointer deleteMe) {
        std::shared_ptr<HE_edge> start = deleteMe->edge;

        std::shared_ptr<HE_edge> currentEdge = start;
        do {
            EdgeList::iterator position = std::find(this->edges.begin(), this->edges.end(), currentEdge);
            std::shared_ptr<HE_edge> neighborEdge = currentEdge->pair;
            neighborEdge->pair = nullptr;
            this->edges.erase(position);
            currentEdge = currentEdge->next;
        } while (currentEdge != start);

    }

    void HE_Wrapper::addVert(VertPointer vert) {
        this->verts.push_back(vert);
    }

    void HE_Wrapper::addEdge(EdgePointer edge) {
        this->edges.push_back(edge);
    }

    void HE_Wrapper::addFace(FacePointer face) {
        this->faces.push_back(face);
    }

    const VertList &HE_Wrapper::getVerts() const {
        return this->verts;
    }

    const EdgeList &HE_Wrapper::getEdges() const {
        return this->edges;
    }

    const FaceList &HE_Wrapper::getFaces() const {
        return this->faces;
    }

}