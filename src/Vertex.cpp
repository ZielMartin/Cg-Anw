//
// Created by Johannes on 28.04.2018.
//

#include "Vertex.h"

Vertex::Vertex(GLfloat const &x, GLfloat const &y, GLfloat const &z){
    this->x(x).y(y).z(z).hk(1.0);
}

Vertex& Vertex::moveAbsolute(GLfloat const &x, GLfloat const &y, GLfloat const &z){
    this->x(x).y(y).z(z);
}

Vertex& Vertex::moveRelative(GLfloat const &x, GLfloat const &y, GLfloat const &z){
    this->x(x+this->x()).y(y+this->y()).z(z+this->z());
}

const glm::vec3 &Vertex::getColor() const {
    return color;
}

void Vertex::setColor(const glm::vec3 &color) {
    Vertex::color = color;
}

