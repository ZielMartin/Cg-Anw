//
// Created by Johannes on 29.04.2018.
//

#ifndef CG_ANW_VERTICESWRAPPER_H
#define CG_ANW_VERTICESWRAPPER_H


#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <c++/4.8.3/vector>
#include <c++/4.8.3/memory>
#include "VectorsWrapper.h"
#include <c++/4.8.3/iostream>


class VerticesWrapper {

private:
    /* Here will be the instance stored. */
    static VerticesWrapper *instance;

    /* Private constructor to prevent instancing. */
    VerticesWrapper();

    //bool: true if selected
    std::vector<std::pair<std::shared_ptr<glm::vec4>, bool>> vertices;



public:
    /* Static access method. */
    static VerticesWrapper *getInstance();

    std::shared_ptr<glm::vec4> addVertex(glm::vec3 vertex, bool selected);

    void deleteSelectedVertices();

    std::shared_ptr<glm::vec4> selectVertex(glm::vec3 worldCoordinates, float radius, bool markSelected);

    void moveSelected(glm::vec3 relativeMovement);

    void resetSelected();


    //Getters & Setters
    std::vector<std::pair<std::shared_ptr<glm::vec4>, bool>> *getVertices();

    void setVertices(const std::vector<std::pair<std::shared_ptr<glm::vec4>, bool>> &vertices);


};


#endif //CG_ANW_VERTICESWRAPPER_H
