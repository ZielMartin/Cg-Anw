#include <iostream>
#include <GL/glut.h>
#include <c++/4.8.3/vector>
#include "VerticesWrapper.h"

using namespace std;

VerticesWrapper *vw;


int rotationX = 40, rotationY = 1, rotationZ = 1;

void drawSpheres() {

    for(glm::vec4 *vertex : *vw->getVertices())
    {
        glPushMatrix();
        glColor3f(1, 1, 1);
        glTranslatef(vertex->x,vertex->y, vertex->z);
        cout<<"draw: "<<vertex->x<<endl;
        glutSolidSphere(0.1,100,50);
        glPopMatrix();
    }

}

void drawGrid() {
    int i;
    for (i = 0; i < 40; i++) {
        glPushMatrix();
        if (i < 20) { glTranslatef(0, 0, i); }
        if (i >= 20) { glTranslatef(i - 20, 0, 0); glRotatef(-90, 0, 1, 0); }
        glBegin(GL_LINES);
        glColor3f(1, 1, 1); glLineWidth(1);
        glVertex3f(0, -0.1, 0); glVertex3f(19, -0.1, 0);
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

    drawSpheres();
    glutSwapBuffers();
}

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1.0f, 0.1f, 1000);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2, 0.2, 0.2, 1);

}



void keyboard(unsigned char key, int x, int y) {

    if (key == 'w') { vw->moveSelected(glm::vec3(0.0,0.0,-1.0)); }
    if (key == 's') { vw->moveSelected(glm::vec3(0.0,0.0,1.0)); }

    if (key == 'a') { vw->moveSelected(glm::vec3(-1.0,0.0,0.0)); }
    if (key == 'd') { vw->moveSelected(glm::vec3(1.0,0.0,0.0)); }

    if (key == 'q') { vw->moveSelected(glm::vec3(0.0,-1.0,0.0)); }
    if (key == 'z') { vw->moveSelected(glm::vec3(0.0,1.0,0.0)); }


    glutPostRedisplay();
}

void mouseClicks(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        glm::vec4 *newVertex = new glm::vec4(1.0,1.0,1.0,1.0);
        vw->addVertex(newVertex);

        vector<glm::vec4 *>selected;
        selected.push_back(newVertex);
        vw->setSelectedVertices(&selected);

        cout<<"click: "<<newVertex->x<<endl;

    }
}



int main(int argc, char** argv) {
    vw = VerticesWrapper::getInstance();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouseClicks);
    glutMainLoop();
    return 0;
}