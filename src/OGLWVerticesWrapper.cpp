//
// Created by Johannes on 29.04.2018.
//

#include "OGLWVerticesWrapper.h"

using namespace std;
using namespace glm;

/* Null, because instance will be initialized on demand. */
OGLWVerticesWrapper *OGLWVerticesWrapper::instance = 0;

OGLWVerticesWrapper *OGLWVerticesWrapper::getInstance() {
    if (instance == 0) {
        instance = new OGLWVerticesWrapper();
    }

    return instance;
}

OGLWVerticesWrapper::OGLWVerticesWrapper() {}

vector<pair<shared_ptr<vec4>, bool>> &OGLWVerticesWrapper::getVertices() {
    return vertices;
}

void OGLWVerticesWrapper::setVertices(const vector<pair<shared_ptr<vec4>, bool>> &vertices) {
    OGLWVerticesWrapper::vertices = vertices;
}

shared_ptr<vec4> OGLWVerticesWrapper::selectVertex(vec3 worldCoordinates, float radius, bool markSelected) {
    for (int i = 0; i < getVertices().size(); i++) {

        shared_ptr<vec4> vertex = getVertices().at(i).first;
        bool *selected = &getVertices().at(i).second;
        if (vertex->x - worldCoordinates.x < radius && vertex->x - worldCoordinates.x > -radius
            && vertex->y - worldCoordinates.y < radius && vertex->y - worldCoordinates.y > -radius
            && vertex->z - worldCoordinates.z < radius && vertex->z - worldCoordinates.z > -radius) {
            markSelected?*selected = !*selected:0;
            return vertex;

        }
    }
    return nullptr;
}

void OGLWVerticesWrapper::resetSelected() {
    for (int i = 0; i < getVertices().size(); i++) {
        getVertices().at(i).second = false;
    }
}


shared_ptr<vec4> OGLWVerticesWrapper::addVertex(vec3 vertex, bool selected = false) {
    vec4 v(vertex, 1.0);
    shared_ptr<vec4> vp = make_shared<vec4>(v);
    pair<shared_ptr<vec4>, bool> pair(vp,selected);
    OGLWVerticesWrapper::vertices.push_back(pair);
    return vp;
}

/*
 * very inefficient, find better solution!
 */
void OGLWVerticesWrapper::deleteSelectedVertices() {
    for (int i = 0; i < getVertices().size(); i++) {
        if (getVertices().at(i).second) {
            OGLWEdgesWrapper *vecW = OGLWEdgesWrapper::getInstance();
            vecW->deleteEdge(getVertices().at(i).first);
            getVertices().at(i).first.reset();
            getVertices().erase(getVertices().begin() + i);
            i--;
        }
    }
}


void OGLWVerticesWrapper::moveSelected(vec3 relativeMovement) {
    for (int i = 0; i < vertices.size(); i++) {
        {
            if (vertices[i].second) {
                pair<shared_ptr<vec4>, bool> vertex = getVertices().at(i);
                vertex.first.get()->x += relativeMovement.x;
                vertex.first.get()->y += relativeMovement.y;
                vertex.first.get()->z += relativeMovement.z;
            }


        }
    }
}
