//
// Created by ZielM on 30.04.2018.
//

#include "HalfEdge.h"
#include <algorithm>
#include <iostream>

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
            EdgePointer neighborEdge = currentEdge->pair;
            if (neighborEdge != nullptr) neighborEdge->pair = nullptr;
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

    FacePointer HE_Wrapper::createFace(VertList &verts) {
        FacePointer newFace = FacePointer(new HE_face);
        this->addFace(newFace);

//        try {
//            checkOrientation(verts);
//        } catch (std::runtime_error &ex) {
//            std::cerr << ex.what() << std::endl;
//        }
        EdgeList edges = createEdgesFromVerts(verts, newFace);

        for (EdgePointer currentEdge : edges) {
            if (currentEdge->pair == nullptr) {
                VertPointer outgoingFrom = currentEdge->vert;
                VertPointer pointingTo = currentEdge->next->vert;

                EdgePointer pairFound = nullptr;
                std::map<VertPointer, EdgeList >::iterator it = accelerationStruct.find(pointingTo);
                if (it != accelerationStruct.end()) {
                    EdgeList &outgoingEdgesFromDestination = it->second;

                    pairFound = findPairAndSetIt(currentEdge, outgoingEdgesFromDestination);
                    if (pairFound) {
                        EdgeList::iterator begin = outgoingEdgesFromDestination.begin();
                        EdgeList::iterator end = outgoingEdgesFromDestination.end();
                        EdgeList::iterator nowHasPair = std::find(begin, end, pairFound);
                        outgoingEdgesFromDestination.erase(nowHasPair);
                    }
                }
                if (!pairFound) {
                    accelerationStruct[outgoingFrom].push_back(currentEdge);
                }
            }
        }
        return newFace;
    }

    void HE_Wrapper::checkOrientation(VertList &verts) {
        bool reversedAlready = true;
        for (VertPointer vert : verts) {
            if (shouldReverse(vert)) {
                if (reversedAlready)
                    throw std::runtime_error("Error in Half-Edge-Struct");
                //std::reverse(verts.begin(), verts.end());
                reversedAlready = true;
            }
        }
    }
    
    bool HE_Wrapper::shouldReverse(const std::shared_ptr<HE_vert> &vert) const {
        EdgeList emanantingFrom = accelerationStruct.find(vert)->second;
        for (EdgePointer edge : emanantingFrom) {
            if (vert == edge->vert) {
                return true;
            }
        }
    }


    EdgeList HE_Wrapper::createEdgesFromVerts(const VertList &verts, FacePointer &newFace) {
        EdgeList edges;
        EdgePointer prev = nullptr;
        for (VertPointer vert : verts) {
            EdgePointer curr = EdgePointer(new HE_edge);
            addEdge(curr);
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
        if (prev != nullptr)
            prev->next = newFace->edge;
        return edges;
    }

    EdgePointer HE_Wrapper::findPairAndSetIt(EdgePointer &currentEdge, EdgeList &outgoingEdgesFromDestination) {
        VertPointer outgoingFrom = currentEdge->vert;
        VertPointer pointingTo = currentEdge->next->vert;
        for (EdgePointer &edgeWithoutPair : outgoingEdgesFromDestination) {
            if (edgeWithoutPair->next->vert == outgoingFrom && edgeWithoutPair->vert == pointingTo) {
                currentEdge->pair = edgeWithoutPair;
                edgeWithoutPair->pair = currentEdge;
                return edgeWithoutPair;
            }
        }
        return nullptr;
    }

}