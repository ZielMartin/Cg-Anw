//
// Created by Johannes on 30.04.2018.
//

/**
 * User-Interaction-Interface
 * - add point with right-click on grid
 * - select multiple points with ctrl + left-click on point
 * - move point/points arrow-keys and '+','-'
 * - remove selected point/points with r
 *
 * - move camera with with w,a,s,d,q,e
 * - rotate camera with mouse click and drag window
 *
 * - draw a line between two points: alt + left-click on the points one after another
 */

#include <VectorsWrapper.h>
#include "OGLWidgetUI.h"
#include "OGLWidget.h"


void keyboard(unsigned char key, int x, int y) {

    //std::cout << "keyboard: " << key << std::endl;

    switch (key) {
        case 'w':
            camera.Move(FORWARD);
            break;
        case 's':
            camera.Move(BACK);
            break;
        case 'e':
            camera.Move(UP);
            break;
        case 'a':
            camera.Move(LEFT);
            break;
        case 'q':
            camera.Move(DOWN);
            break;
        case 'd':
            camera.Move(RIGHT);
            break;
        case '-':            //DOWN
            vw->moveSelected(glm::vec3(0.0, -MOVESTEPSIZE, 0.0));
            break;
        case '+':            //UP
            vw->moveSelected(glm::vec3(0.0, MOVESTEPSIZE, 0.0));
            break;
        case 'r':
            vw->deleteSelectedVertices();
        case 'g':
            grid = !grid;
        default:
            break;
    }

    glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y) {
    //std::cout << "specialKeyboard: " << key << std::endl;
    switch (key) {
        case 101:              //UP ARROW
            vw->moveSelected(glm::vec3(-MOVESTEPSIZE, 0.0, 0.0));
            break;
        case 100:            //LEFT ARROW
            vw->moveSelected(glm::vec3(0.0, 0.0, MOVESTEPSIZE));
            break;
        case 103:            //DOWN ARROW
            vw->moveSelected(glm::vec3(MOVESTEPSIZE, 0.0, 0.0));
            break;
        case 102:            //RIGHT ARROW
            vw->moveSelected(glm::vec3(0.0, 0.0, -MOVESTEPSIZE));
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

void mouseClicks(int button, int state, int x, int y) {
    camera.SetPos(button, state, x, y);
    glm::vec3 worldCoordinates = getWorldCoordinates(x, y);

    static std::shared_ptr<glm::vec4> vertA;
    static std::shared_ptr<glm::vec4> vertB;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && glutGetModifiers() == GLUT_ACTIVE_CTRL) {

        vw->selectVertex(worldCoordinates, SPHERERADIUS, true);

    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && glutGetModifiers() != GLUT_ACTIVE_ALT) {

        vw->resetSelected();

        vw->addVertex(worldCoordinates, true);
        //std::cout << "addvertex"  << std::endl;
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && glutGetModifiers() == GLUT_ACTIVE_ALT) {

        //first define vertA
        if (vertA == nullptr) {
            vertA = vw->selectVertex(worldCoordinates, SPHERERADIUS, false);

        } else if (vertB == nullptr) {  //then vertB
            vertB = vw->selectVertex(worldCoordinates, SPHERERADIUS, false);

            if (vertB.get() != vertA.get() && vertB != nullptr) {
                VectorsWrapper *vecW = VectorsWrapper::getInstance();
                vecW->addEdge(vertA, vertB);
            } else {
                //if double click on vertex or no vertex clicked
                vertB = nullptr;
                vertA = nullptr;
            }
        } else {
            vertA = vw->selectVertex(worldCoordinates, SPHERERADIUS, false);
            vertB = nullptr;
        }


    }
}

