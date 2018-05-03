//
// Created by ZielM on 02.05.2018.
//

#ifndef CG_ANW_SIMPLEOBJPARSER_H
#define CG_ANW_SIMPLEOBJPARSER_H

#include "HalfEdge.h"

namespace cg {
    class SimpleObjParser {
    public:
        SimpleObjParser();

        std::shared_ptr<HE_Wrapper> create(std::vector<glm::vec3>, std::vector<std::vector<int>>);

    private:
    };

}
#endif //CG_ANW_SIMPLEOBJPARSER_H
