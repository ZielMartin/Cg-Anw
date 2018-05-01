//
// Created by Johannes on 01.05.2018.
//

#ifndef CG_ANW_VECTORSWRAPPER_H
#define CG_ANW_VECTORSWRAPPER_H


#include <glm/vec4.hpp>
#include <c++/4.8.3/utility>
#include <c++/4.8.3/vector>
#include <c++/4.8.3/memory>


class VectorsWrapper {

private:
    /* Here will be the instance stored. */
    static VectorsWrapper *instance;

    /* Private constructor to prevent instancing. */
    VectorsWrapper();

    std::vector<std::shared_ptr<glm::vec4>> edges;




public:
    /* Static access method. */
    static VectorsWrapper *getInstance();

    void addEdge(std::shared_ptr<glm::vec4> vert);


    //Getters & Setters
    std::vector<std::shared_ptr<glm::vec4>> *getEdges();

    void setVertices(const std::vector<std::shared_ptr<glm::vec4>> &edges);
};


#endif //CG_ANW_VECTORSWRAPPER_H
