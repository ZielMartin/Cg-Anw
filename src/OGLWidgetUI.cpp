//
// Created by Johannes on 30.04.2018.
//

#include "OGLWidgetUI.h"
#include "OGLWidget.h"



void keyboard(unsigned char key, int x, int y) {

    if (key == 'w') { vw->moveSelected(glm::vec3(0.0, 0.0, -1.0)); }
    if (key == 's') { vw->moveSelected(glm::vec3(0.0, 0.0, 1.0)); }

    if (key == 'a') { vw->moveSelected(glm::vec3(-1.0, 0.0, 0.0)); }
    if (key == 'd') { vw->moveSelected(glm::vec3(1.0, 0.0, 0.0)); }

    if (key == 'q') { vw->moveSelected(glm::vec3(0.0, -1.0, 0.0)); }
    if (key == 'z') { vw->moveSelected(glm::vec3(0.0, 1.0, 0.0)); }


    glutPostRedisplay();
}

void mouseClicks(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        if (vw->getVertices()->size() >= 1) {
            vw->getVertices()->at(vw->getVertices()->size() - 1).second = false;

        }


        glm::vec4 *newVertex = new glm::vec4(getWorldCoordinates(x, y), 1.0);
        vw->addVertex(newVertex, true);


    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && glutGetModifiers() == GLUT_ACTIVE_CTRL) {

    }
}