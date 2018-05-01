//
// Created by ZielM on 30.04.2018.
//

#include "HalfEdge.h"

namespace cg {
    void HE_Wrapper::clear() {
        this->clear();
    }

    void HE_Wrapper::deleteVert(std::shared_ptr<HE_vert> deleteMe) {

    }

    void HE_Wrapper::deleteEdge(std::shared_ptr<HE_edge> deleteMe) {

    }

    void HE_Wrapper::deleteFace(std::shared_ptr<HE_face> deleteMe) {
        std::shared_ptr<HE_edge> start = deleteMe->edge;

        std::shared_ptr<HE_edge> currentEdge = start;
        do {
            std::shared_ptr<HE_edge> neighborEdge = currentEdge->pair;
            neighborEdge->pair = nullptr;
            currentEdge = currentEdge->next;
        } while (currentEdge != start);

    }

}