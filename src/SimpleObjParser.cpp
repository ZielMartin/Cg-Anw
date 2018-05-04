//
// Created by ZielM on 02.05.2018.
//

#include <iostream>
#include "SimpleObjParser.h"

namespace cg {

    SimpleObjParser::SimpleObjParser() {
        std::cout << "SimpleObjParser";
    }

    std::shared_ptr<HE_Wrapper>
    SimpleObjParser::create(std::vector<glm::vec3> verts, std::vector<std::vector<int>> faces) {
        HE_Wrapper *he_wrapper = new HE_Wrapper();

        for (glm::vec3 &vert : verts) {
            he_wrapper->createVert(glm::vec4(vert, 1.0));
        }

        EdgeList beschleunigungsListe = EdgeList();

        for (std::vector<int> &faceIndices : faces) {
            size_t faceListSize = he_wrapper->getFaces().size();
            VertList vertsBelongingToFace;
            for (int index : faceIndices) {
                vertsBelongingToFace.push_back(he_wrapper->getVerts()[index]);
            }
            he_wrapper->createFace(vertsBelongingToFace);
        }
        return std::shared_ptr<HE_Wrapper>(he_wrapper);
    }

}