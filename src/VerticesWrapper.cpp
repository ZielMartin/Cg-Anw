//
// Created by Johannes on 29.04.2018.
//

#include <c++/4.8.3/iostream>
#include "VerticesWrapper.h"


/* Null, because instance will be initialized on demand. */
VerticesWrapper *VerticesWrapper::instance = 0;

VerticesWrapper *VerticesWrapper::getInstance() {
    if (instance == 0) {
        instance = new VerticesWrapper();
    }

    return instance;
}

VerticesWrapper::VerticesWrapper() {}

std::vector<std::pair<glm::vec4 *, bool>> *VerticesWrapper::getVertices() {
    return &vertices;
}

void VerticesWrapper::setVertices(const std::vector<std::pair<glm::vec4 *, bool>> &vertices) {
    VerticesWrapper::vertices = vertices;
}

bool VerticesWrapper::selectVertex(glm::vec3 *worldCoordinates, float radius) {
    for (int i = 0; i < getVertices()->size(); i++) {
        glm::vec4 *vertex = getVertices()->at(i).first;
        bool *selected = &getVertices()->at(i).second;
        if (vertex->x - worldCoordinates->x < radius && vertex->x - worldCoordinates->x > -radius
            && vertex->y - worldCoordinates->y < radius && vertex->y - worldCoordinates->y > -radius
            && vertex->z - worldCoordinates->z < radius && vertex->z - worldCoordinates->z > -radius) {
            *selected = !*selected;
            return true;
            break;
        }
    }
    return false;
}

void VerticesWrapper::resetSelected() {
    for (int i = 0; i < getVertices()->size(); i++) {
        getVertices()->at(i).second = false;
    }
}


std::vector<std::pair<glm::vec4 *, bool>> *VerticesWrapper::addVertex(glm::vec3 vertex, bool selected) {
    VerticesWrapper::vertices.push_back(std::pair<glm::vec4 *, bool>(new glm::vec4(vertex, 1.0), selected));
    return &vertices;
}

std::vector<std::pair<glm::vec4 *, bool>> *VerticesWrapper::deleteSelectedVertices() {
    for (int i = 0; i < getVertices()->size(); i++) {
        if (getVertices()->at(i).second) {
            std::pair<glm::vec4 *, bool> vertex = getVertices()->at(i);
            delete vertex.first;
            getVertices()->erase(getVertices()->begin() + i);
            i--;
        }
    }
    return &vertices;
}


void VerticesWrapper::moveSelected(glm::vec3 relativeMovement) {
    for (int i = 0; i < vertices.size(); i++) {
        {
            if (vertices[i].second) {
                glm::vec4 *vertex = vertices[i].first;
                vertex->x += relativeMovement.x;
                vertex->y += relativeMovement.y;
                vertex->z += relativeMovement.z;
            }


        }
    }
}
