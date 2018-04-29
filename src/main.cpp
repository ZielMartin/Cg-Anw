#include <iostream>
#include <GL/glut.h>
#include <c++/4.8.3/vector>
#include "VerticesWrapper.h"

using namespace std;

VerticesWrapper *vw;


int rotationX = 40, rotationY = 1, rotationZ = 1;

glm::vec3 background(0.2, 0.2, 0.2);

void drawSpheres() {
    for(glm::vec4 *vertex : *vw->getVertices())
    {
        glPushMatrix();
        glColor3f(1, 1, 1);
        glTranslatef(vertex->x,vertex->y, vertex->z);
        glutSolidSphere(0.1,100,50);
        glPopMatrix();
    }

}

/**
 * same color as background, grid underlying square.
 * necessary because mouse position is only correct detected while clicking on objects
 */
//TODO: make invisible
void drawInvisiblePane(){
    glPushMatrix();
    glColor3f(background.r,background.g,background.b);
    glBegin(GL_POLYGON);
    glVertex3f(0, -0.1, 0);
    glVertex3f(19, -0.1, 0);
    glVertex3f(19, -0.1, 19);
    glVertex3f(0, -0.1, 19);

    glEnd();
    glPopMatrix();

}

void drawGrid() {
    int i;
    for (i = 0; i < 40; i++) {
        glPushMatrix();
        if (i < 20) { glTranslatef(0, 0, i); }
        if (i >= 20) { glTranslatef(i - 20, 0, 0); glRotatef(-90, 0, 1, 0); }
        glLineWidth(1);
        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(19, 0, 0);
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

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1.0f, 0.1f, 1000);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(background.r,background.g,background.b,1);
    glEnable(GL_DEPTH_TEST);
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
glm::vec3 getWorldCoordinates(int x, int y){
    double objx, objy, objz;
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &objx, &objy, &objz);

    cout << objx << " , " << objy << " , " << objz << "\n";
    
    return glm::vec3(objx,objy,objz);

}

void mouseClicks(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        glm::vec4 *newVertex = new glm::vec4(getWorldCoordinates(x,y),1.0);
        vw->addVertex(newVertex);

        vector<glm::vec4 *>selected;
        selected.push_back(newVertex);
        vw->setSelectedVertices(&selected);

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











