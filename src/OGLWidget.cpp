//
// Created by Johannes on 30.04.2018.
//

#include "OGLWidget.h"
#include "OGLWidgetUI.h"


using namespace std;


VerticesWrapper *vw = VerticesWrapper::getInstance();
glm::vec3 background = (glm::vec3(0.2, 0.2, 0.2));
glm::vec3 selectedSphereColor = (glm::vec3(1, 0, 0));
glm::vec3 sphereColor = (glm::vec3(1, 1, 1));
glm::vec3 gridColor = (glm::vec3(1, 1, 1));
int rotationX = 40, rotationY = 1, rotationZ = 1;


void drawSpheres() {
    for (int i = 0; i < vw->getVertices()->size(); i++) {
        glm::vec4 *vertex = vw->getVertices()->at(i).first;
        glPushMatrix();
        if (vw->getVertices()->at(i).second) {
            glColor3f(selectedSphereColor.r, selectedSphereColor.g, selectedSphereColor.b);
        } else {
            glColor3f(sphereColor.r, sphereColor.g, sphereColor.b);

        }

        glTranslatef(vertex->x, vertex->y, vertex->z);
        glutSolidSphere(SPHERERADIUS, SPHERESLICES, SPHERESTACKS);
        glPopMatrix();
    }

}

/**
 * same color as background, grid underlying square.
 * necessary because mouse position is only correct detected while clicking on objects
 */
//TODO: make invisible
void drawInvisiblePane() {
    glPushMatrix();
    glColor3f(background.r, background.g, background.b);
    glBegin(GL_POLYGON);
    glVertex3f(0, -0.1, 0);
    glVertex3f(GRIDLENGTH, -0.1, 0);
    glVertex3f(GRIDLENGTH, -0.1, GRIDLENGTH);
    glVertex3f(0, -0.1, GRIDLENGTH);

    glEnd();
    glPopMatrix();

}

void drawGrid() {
    int i;
    for (i = 0; i < GRIDLENGTH*2; i++) {
        glPushMatrix();
        if (i < GRIDLENGTH) { glTranslatef(0, 0, i); }
        if (i >= GRIDLENGTH) {
            glTranslatef(i - GRIDLENGTH, 0, 0);
            glRotatef(-90, 0, 1, 0);
        }
        glLineWidth(1);
        glBegin(GL_LINES);
        glColor3f(gridColor.r, gridColor.g, gridColor.b);
        glVertex3f(0, 0, 0);
        glVertex3f(GRIDLENGTH-1, 0, 0);
        glEnd();
        glPopMatrix();
    }
}


void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-13, 0, -45);
    glRotatef(rotationX, rotationY, rotationZ, 0);
    drawGrid();
    drawInvisiblePane();
    drawSpheres();
    glutSwapBuffers();
}

void initOGLWidget(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(WIDOWWIDTH, WIDOWHEIGHT);
    glutCreateWindow("");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1.0f, 0.1f, 1000);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(background.r, background.g, background.b, 1);
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouseClicks);
    glutMainLoop();
}



glm::vec3 getWorldCoordinates(int x, int y) {
    double objx, objy, objz;
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    winX = (float) x;
    winY = (float) viewport[3] - (float) y;
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &objx, &objy, &objz);

    cout << objx << " , " << objy << " , " << objz << "\n";

    return glm::vec3(objx, objy, objz);

}



















