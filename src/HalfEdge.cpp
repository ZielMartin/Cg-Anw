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

    VertPointer HE_Wrapper::createVert(glm::vec4 pos) {
        VertPointer newVert = VertPointer(new HE_vert(pos));
        this->verts.push_back(newVert);
        return newVert;
    }

    FacePointer HE_Wrapper::createFace(VertList verts) {
        FacePointer newFace = FacePointer(new HE_face);
        this->addFace(newFace);
        EdgePointer prev = nullptr;

        EdgeList edges;

        for (VertPointer vert : verts) {
            EdgePointer curr = EdgePointer(new HE_edge);
            this->addEdge(curr);
            edges.push_back(curr);

            curr->vert = vert;
            curr->face = newFace;

            if (vert->edge == nullptr)
                vert->edge = curr;

            if (newFace->edge == nullptr)
                newFace->edge = curr;

            if (prev != nullptr)
                prev->next = curr;

            prev = curr;
        }
        prev->next = newFace->edge;

        for (EdgePointer currentEdge : edges) {
            if (currentEdge->pair == nullptr) {

                VertPointer emanatingFrom = currentEdge->vert;
                VertPointer pointingTo = currentEdge->next->vert;

                BeschleunigungsStruktur::iterator it;
                it = beschleunigungsStruktur.find(pointingTo);

                if (it != beschleunigungsStruktur.end()) {
                    EdgeList &edgesEmanatingFromVert = (*it).second;
                    bool matchFound = false;

                    for (EdgeList::iterator iterator = edgesEmanatingFromVert.begin();
                         iterator != edgesEmanatingFromVert.end();
                         iterator++) {
                        EdgePointer emanatingEdge = *iterator;
                        if (emanatingEdge->vert == pointingTo &&
                            emanatingFrom == emanatingEdge->next->vert) {
                            matchFound = true;

                            emanatingEdge->pair = currentEdge;
                            currentEdge->pair = emanatingEdge;

                            edgesEmanatingFromVert.erase(iterator);
                        }
                    }

                    if (!matchFound) {
                        beschleunigungsStruktur[emanatingFrom].push_back(currentEdge);
                    }
                }
            }
        }
    }

}