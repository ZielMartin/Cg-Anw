//
// Created by Johannes on 30.04.2018.
//

#ifndef CG_ANW_OGLWIDGET_H
#define CG_ANW_OGLWIDGET_H

#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include "camera.h"
#include "HalfEdge.h"


#define SPHERERADIUS 0.1
#define SPHERESLICES 100
#define SPHERESTACKS 50
#define GRIDLENGTH 50
#define WIDOWWIDTH 1024
#define WIDOWHEIGHT 600

namespace cg {
    extern std::shared_ptr<cg::HE_Wrapper> wrapperPtr;
    extern cg::Camera camera;

    extern bool grid;


    void initOGLWidget(int argc, char **argv, std::shared_ptr<cg::HE_Wrapper> wrapper);

    glm::vec3 getWorldCoordinates(int x, int y);

    void drawSpheres();

    void drawFaces();

    void drawGridPane();

    void drawGrid();

    void display();
}

#endif //CG_ANW_OGLWIDGET_H
