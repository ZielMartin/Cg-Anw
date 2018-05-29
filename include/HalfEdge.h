/*
 * HalfEdge.h
 *
 *  Created on: 13.06.2017
 *      Author: martin
 */

#ifndef INCLUDE_HALFEDGE_H_
#define INCLUDE_HALFEDGE_H_

#include <ostream>
#include <list>
#include <glm/glm.hpp>
#include <memory>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include "NormalCalculation.h"

#include "OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh"

namespace cg {
    struct MyTraits {
        typedef glm::vec4 Point;
        typedef glm::vec4 Normal;
        typedef float Scalar;
    };
    typedef OpenMesh::PolyMesh_ArrayKernelT<MyTraits> MyMesh;


    class HE_Wrapper {
    public:
        HE_Wrapper();

        void addVert(glm::vec4);
        void createFace();
    private:
        MyMesh mesh;

    };
}
#endif /* INCLUDE_HALFEDGE_H_ */
