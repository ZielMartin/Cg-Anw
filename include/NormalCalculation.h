//
// Created by Johannes on 12.05.2018.
//

#ifndef CG_ANW_NORMALSWRAPPER_H
#define CG_ANW_NORMALSWRAPPER_H

#include "HalfEdge.h"
#include <iostream>




namespace cg {

    void calcNormalsOnMove(std::shared_ptr<struct HE_vert> vert);

    void calcFaceNormal(std::shared_ptr<struct HE_face> face);

    void calcVertexNormal(std::shared_ptr<struct HE_vert> vert);
}

#endif //CG_ANW_NORMALSWRAPPER_H
