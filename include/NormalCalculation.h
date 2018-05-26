//
// Created by Johannes on 12.05.2018.
//

#ifndef CG_ANW_NORMALSWRAPPER_H
#define CG_ANW_NORMALSWRAPPER_H


#include <memory>
#include "HalfEdge.h"
#include <iostream>
#include <glm/glm.hpp>
#include <memory>

namespace cg {

    class NormalCalculation {

    public:

        static void recalcNormalsAroundVertex(std::map<std::shared_ptr<struct HE_face>,std::vector<std::shared_ptr<struct HE_edge>>>);

    private:

        static void calcFaceNormal(std::shared_ptr<struct HE_face> face);

        static void calcVertexNormal(std::shared_ptr<struct HE_vert> vert);

    };
}

#endif //CG_ANW_NORMALSWRAPPER_H
