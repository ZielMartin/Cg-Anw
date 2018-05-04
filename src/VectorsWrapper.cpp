//
// Created by Johannes on 01.05.2018.
//


#include "VectorsWrapper.h"

using namespace std;
using namespace glm;


/* Null, because instance will be initialized on demand. */
VectorsWrapper *VectorsWrapper::instance = 0;

VectorsWrapper *VectorsWrapper::getInstance() {
    if (instance == 0) {
        instance = new VectorsWrapper();
    }

    return instance;
}

VectorsWrapper::VectorsWrapper() {}


void VectorsWrapper::addEdge(shared_ptr<vec4> vertA, shared_ptr<vec4> vertB){
    pair<shared_ptr<vec4>, shared_ptr<vec4>> pair(vertA, vertB);
    edges.push_back(pair);
}

void VectorsWrapper::deleteEdge(shared_ptr<vec4> vert){
    for(int j = 0; j < getEdges().size(); j++){
        if(getEdges().at(j).first == vert || getEdges().at(j).second == vert ){
            getEdges().at(j).first.reset();
            getEdges().at(j).second.reset();
            getEdges().erase(getEdges().begin() + j);
            j--;
        }
    }
}



//Getters & Setters
vector<pair<shared_ptr<vec4>, shared_ptr<vec4>>> &VectorsWrapper::getEdges(){
    return edges;
}

void VectorsWrapper::setVertices(const vector<pair<shared_ptr<vec4>, shared_ptr<vec4>>> &edges){
    this->edges = edges;
}