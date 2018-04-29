//
// Created by Johannes on 29.04.2018.
//

#include <c++/4.8.3/iostream>
#include "VerticesWrapper.h"



/* Null, because instance will be initialized on demand. */
VerticesWrapper* VerticesWrapper::instance = 0;

VerticesWrapper* VerticesWrapper::getInstance()
{
    if (instance == 0)
    {
        instance = new VerticesWrapper();
    }

    return instance;
}

VerticesWrapper::VerticesWrapper() {}

std::vector<std::pair<glm::vec4*,bool>> *VerticesWrapper::getVertices()  {
    return &vertices;
}

void VerticesWrapper::setVertices(const std::vector<std::pair<glm::vec4*,bool>> &vertices) {
    VerticesWrapper::vertices = vertices;
}





std::vector<std::pair<glm::vec4*,bool>> *VerticesWrapper::addVertex(glm::vec4 *vertex, bool selected)  {
    VerticesWrapper::vertices.push_back(std::pair<glm::vec4*,bool> (vertex, selected));
    return &vertices;
}

std::vector<std::pair<glm::vec4*,bool>> *VerticesWrapper::deleteVertex(glm::vec4 *vertex)  {
    return &vertices;
}




void VerticesWrapper::moveSelected(glm::vec3 relativeMovement) {
    for(int i  = 0; i < vertices.size(); i ++){
    {
        if(vertices[i].second) {
            glm::vec4 *vertex = vertices[i].first;
            vertex->x += relativeMovement.x;
            vertex->y += relativeMovement.y;
            vertex->z += relativeMovement.z;
        }


    }
}
}
