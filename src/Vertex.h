//
// Created by Johannes on 28.04.2018.
//

#ifndef CG_ANW_VERTEX_H
#define CG_ANW_VERTEX_H


#include <GL/gl.h>
#include <glm/vec3.hpp>

class Vertex {
public:
    Vertex(GLfloat const &x, GLfloat const &y, GLfloat const &z);

    //Getter
    const GLfloat& x() const;
    const GLfloat& y() const;
    const GLfloat& z() const;
    const GLfloat& hk() const;
    const glm::vec3 &getColor() const;


    //Setter
    Vertex& hk(const GLfloat& hk);
    void setColor(const glm::vec3 &color);



    Vertex& moveAbsolute(GLfloat const &x, GLfloat const &y, GLfloat const &z);
    Vertex& moveRelative(GLfloat const &x, GLfloat const &y, GLfloat const &z);


private:
    glm::vec3 color;


private:
    //Setter
    Vertex& x(const GLfloat& x);
    Vertex& y(const GLfloat& y);
    Vertex& z(const GLfloat& z);

};


#endif //CG_ANW_VERTEX_H
