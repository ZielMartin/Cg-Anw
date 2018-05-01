//
// Created by Johannes on 30.04.2018.
//

/**
 * User-Interaction-Interface
 * - add point with right-click on grid
 * - select multiple points with ctrl + right-click on point
 * - move point/points arrow-keys and '+','-'
 * - remove selected point/points with r
 *
 * - move camera with with w,a,s,d,q,e
 * - rotate camera with mouse click and drag window
 */

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
    glm::vec3 worldCoordinates = getWorldCoordinates(x, y);

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && glutGetModifiers() == GLUT_ACTIVE_CTRL) {

        vw->selectVertex(worldCoordinates, SPHERERADIUS);

    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

        vw->resetSelected();

        vw->addVertex(worldCoordinates, true);
        //std::cout << "addvertex"  << std::endl;
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        camera.SetPos(button, state, x, y);
        //std::cout << "setpos"  << std::endl;

    }
}

