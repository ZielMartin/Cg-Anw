//
// Created by Johannes on 01.05.2018.
//

#ifndef CG_ANW_VECTORSWRAPPER_H
#define CG_ANW_VECTORSWRAPPER_H


#include <glm/vec4.hpp>
#include <utility>
#include <vector>
#include <memory>


class OGLWEdgesWrapper {

private:
    /* Here will be the instance stored. */
    static OGLWEdgesWrapper *instance;

    /* Private constructor to prevent instancing. */
    OGLWEdgesWrapper();

    std::vector<std::pair<std::shared_ptr<glm::vec4>, std::shared_ptr<glm::vec4>>> edges;




public:
    /* Static access method. */
    static OGLWEdgesWrapper *getInstance();

    void addEdge(std::shared_ptr<glm::vec4> vertA, std::shared_ptr<glm::vec4> vertB);

    void deleteEdge(std::shared_ptr<glm::vec4> vert);


    //Getters & Setters
    std::vector<std::pair<std::shared_ptr<glm::vec4>, std::shared_ptr<glm::vec4>>> &getEdges();

    void setVertices(const std::vector<std::pair<std::shared_ptr<glm::vec4>, std::shared_ptr<glm::vec4>>> &edges);
};


#endif //CG_ANW_VECTORSWRAPPER_H
