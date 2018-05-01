//
// Created by Johannes on 01.05.2018.
//

/**
 * example:
 *
 */

#include "VectorsWrapper.h"

/* Null, because instance will be initialized on demand. */
VectorsWrapper *VectorsWrapper::instance = 0;

VectorsWrapper *VectorsWrapper::getInstance() {
    if (instance == 0) {
        instance = new VectorsWrapper();
    }

    return instance;
}

VectorsWrapper::VectorsWrapper() {}

/**
 * use shared_ptr if vertices already in VerticesWrapper.vertices
 */
void VectorsWrapper::addEdge(std::shared_ptr<glm::vec4> vert){
    edges.push_back(vert);
}





//Getters & Setters
std::vector<std::shared_ptr<glm::vec4>> *VectorsWrapper::getEdges(){
    return &edges;
}

void VectorsWrapper::setVertices(const std::vector<std::shared_ptr<glm::vec4>> &edges){
    this->edges = edges;
}