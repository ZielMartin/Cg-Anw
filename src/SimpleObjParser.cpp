//
// Created by ZielM on 02.05.2018.
//

#include <iostream>
#include "SimpleObjParser.h"

namespace cg {

    SimpleObjParser::SimpleObjParser() {
        std::cout << "SimpleObjParser";
    }

    std::shared_ptr<HE_Wrapper> SimpleObjParser::create(std::vector<glm::vec3> verts, std::vector<std::vector<int>> faces) {
        std::shared_ptr<HE_Wrapper> he_wrapper = std::shared_ptr<HE_Wrapper>(new HE_Wrapper);

        for (glm::vec3 &vert : verts) {
            he_wrapper->addVert(VertPointer(new HE_vert(vert)));
        }

        EdgeList beschleunigungsListe = EdgeList();

        for (std::vector<int> &faceIndices : faces) {
            FacePointer newFace = FacePointer(new HE_face);
            he_wrapper->addFace(newFace);
            EdgePointer prev = nullptr;

            for (int &num : faceIndices) {
                EdgePointer curr = EdgePointer(new HE_edge);
                VertPointer edgeStartingFrom = he_wrapper->getVerts()[num];

                he_wrapper->addEdge(curr);
                curr->vert = edgeStartingFrom;
                curr->face = newFace;

                if (edgeStartingFrom->edge == nullptr)
                    edgeStartingFrom->edge = curr;

                if (newFace->edge == nullptr)
                    newFace->edge = curr;

                if (prev != nullptr)
                    prev->next = curr;

                prev = curr;
            }
            prev->next = newFace->edge;
        }

        for (EdgePointer currentEdge : he_wrapper->getEdges()) {
            if (currentEdge->pair == nullptr) {

                VertPointer pointingTo = currentEdge->next->vert;
                bool matchFound = false;
                for (EdgePointer edgeWithoutPair : beschleunigungsListe) {
                    if (edgeWithoutPair->vert == pointingTo &&
                        currentEdge->vert == edgeWithoutPair->next->vert) {
                        matchFound = true;
                        currentEdge->pair = edgeWithoutPair;
                        edgeWithoutPair->pair = currentEdge;
                    }
                }
                if (!matchFound) {
                    beschleunigungsListe.push_back(currentEdge);
                }
            }
        }
        return he_wrapper;
    }

}