//
// Created by Johannes on 30.04.2018.
//

#ifndef CG_ANW_OGLWIDGET_H
#define CG_ANW_OGLWIDGET_H


#include <iostream>
#include <GL/glut.h>
#include <vector>
#include "VerticesWrapper.h"


#define SPHERERADIUS 0.1
#define SPHERESLICES 100
#define SPHERESTACKS 50
#define GRIDLENGTH 20
#define WIDOWWIDTH 800
#define WIDOWHEIGHT 600




extern VerticesWrapper *vw;
extern glm::vec3 background;
extern glm::vec3 selectedSphereColor;
extern glm::vec3 sphereColor;
extern glm::vec3 gridColor;
extern int rotationX, rotationY, rotationZ;



void initOGLWidget(int argc, char **argv);

glm::vec3 getWorldCoordinates(int x, int y);

void drawSpheres();

void drawInvisiblePane();

void drawGrid();

void display();


#endif //CG_ANW_OGLWIDGET_H
