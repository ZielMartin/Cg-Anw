//
// Created by ZielM on 30.04.2018.
//

#include "HalfEdge.h"


namespace cg {


    HE_Wrapper::HE_Wrapper() {
    }

    void HE_Wrapper::addVert(glm::vec4 vert) {
        this->mesh.new_vertex(vert);
    }

    void HE_Wrapper::createFace() {
        this->mesh.add_face()
    }

}