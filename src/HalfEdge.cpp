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
        EdgeList l = getEdgesFromVert(deleteMe);

        for(EdgePointer edge : l) {
            if(!edge->face->isBoundary) {
                deleteEdge(edge);
            }
        }
    }

    void HE_Wrapper::deleteEdge(EdgePointer deleteMe) {
        FacePointer faceA = deleteMe->face;
        FacePointer faceB = deleteMe->pair->face;
        faceA->isBoundary = true;

        if(faceA->edge == deleteMe) {
            faceA->edge = deleteMe->next;
        }

        EdgePointer curr = deleteMe->pair;
        do {
            curr->face = faceA;
            curr = curr->next;
        } while (curr != deleteMe->pair);

        EdgePointer prev = findPrevEdge(deleteMe);
        prev->next = deleteMe->pair->next;
        findPrevEdge(deleteMe->pair)->next = deleteMe->next;

        faces.erase(std::find(faces.begin(), faces.end(), faceB));

        FacePointer neighbor = findNeighborThatIsBoundary(faceA);
        while (neighbor) {
            this->deleteEdge(findEdgeConnectingFaces(faceA, neighbor));
            neighbor = findNeighborThatIsBoundary(faceA);
        }

    }

    EdgePointer HE_Wrapper::findPrevEdge(const EdgePointer edge) const {
        EdgePointer curr = edge->next;
        while (curr->next != edge) curr = curr->next;
        return curr;
    }

    void HE_Wrapper::deleteFace(FacePointer deleteMe) {
        if (deleteMe->isBoundary)
            throw new std::invalid_argument("Face already deleted");
        deleteMe->isBoundary = true;

        FacePointer neighborBoundary = findNeighborThatIsBoundary(deleteMe);
        if (neighborBoundary) {
            EdgePointer edge = findEdgeConnectingFaces(neighborBoundary, deleteMe);
            this->deleteEdge(edge);
            return;

        }

    }

    EdgePointer HE_Wrapper::findEdgeConnectingFaces(const FacePointer faceA, const FacePointer faceB) const {
        EdgePointer start = faceA->edge;

        EdgePointer current = start;
        do {
            if (current->pair->face == faceB) {
                return current;
            }
            current = current->next;
        } while (current != start);
        return nullptr;
    }

    FacePointer HE_Wrapper::findNeighborThatIsBoundary(const FacePointer face) const {
        std::shared_ptr<HE_edge> start = face->edge;

        std::shared_ptr<HE_edge> currentEdge = start;
        do {
            FacePointer currentNeighbor = currentEdge->pair->face;
            if (currentNeighbor->isBoundary) {
                return currentNeighbor;
            }
        } while (currentEdge != start);

        return nullptr;
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

    EdgeList HE_Wrapper::getEdgesFromFace(FacePointer face) {
        EdgePointer start = face->edge;
        EdgeList borderingEdges;

        EdgePointer currentEdge = start;
        do {
            borderingEdges.push_back(currentEdge);
            currentEdge = currentEdge->pair->next;
        } while (currentEdge != start);
        return borderingEdges;
    }

    EdgeList HE_Wrapper::getEdgesFromVert(VertPointer vert) {
        EdgePointer start = vert->edge;
        EdgePointer curr = start;
        EdgeList list;
        do {
            list.push_back(curr);
            curr = curr->pair->next;
        } while (curr != start);
        return list;
    }

    void HE_Wrapper::forEveryOutgoingEdgeFromVert(VertPointer vert, std::function<void(EdgePointer)> &func) {
        EdgePointer start = vert->edge;
        EdgePointer curr = start;
        do {
            func(curr);
            curr = curr->pair->next;
        } while (curr != start);
    }

    void HE_Wrapper::forEveryEdgeAroundFace(FacePointer face, std::function<void(EdgePointer)> &func) {
        EdgePointer start = face->edge;
        EdgePointer curr = start;
        do {
            func(curr);
            curr = curr->next;
        } while (curr != start);

    }
}