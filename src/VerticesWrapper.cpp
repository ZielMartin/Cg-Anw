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

const std::vector<glm::vec4 *> *VerticesWrapper::getVertices() const {
    return &vertices;
}

void VerticesWrapper::setVertices(const std::vector<glm::vec4 *> &vertices) {
    VerticesWrapper::vertices = vertices;
}

const std::vector<glm::vec4 *> *VerticesWrapper::getSelectedVertices() const {
    return &selectedVertices;
}

void VerticesWrapper::setSelectedVertices(const std::vector<glm::vec4 *> *selectedVertices) {
    VerticesWrapper::selectedVertices = *selectedVertices;
}




std::vector<glm::vec4 *> *VerticesWrapper::addVertex(glm::vec4 *vertex)  {
    VerticesWrapper::vertices.push_back(vertex);
    return &vertices;
}

std::vector<glm::vec4 *> *VerticesWrapper::deleteVertex(glm::vec4 *vertex)  {
    return &vertices;
}

bool VerticesWrapper::selectVertex(glm::vec4 *vertex){
    VerticesWrapper::selectedVertices.push_back(vertex);
}

void VerticesWrapper::moveSelected(glm::vec3 relativeMovement) {
    for(glm::vec4 *vertex : selectedVertices)
    {
        vertex->x += relativeMovement.x;
        vertex->y += relativeMovement.y;
        vertex->z += relativeMovement.z;


    }
}
