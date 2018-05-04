//
// Created by Johannes on 30.04.2018.
//


#include "OGLWidget.h"
#include "OGLWidgetUI.h"


using namespace std;
using namespace glm;


//Create the Camera
Camera camera;


VerticesWrapper *vw = VerticesWrapper::getInstance();
VectorsWrapper *vecW = VectorsWrapper::getInstance();
vec3 backgroundColor = vec3(0,0,0);
vec3 gridPaneColor = vec3(0.2, 0.2, 0.2);
vec3 selectedSphereColor = vec3(1, 0, 0);
vec3 sphereColor = vec3(1, 1, 1);
vec3 gridColor = vec3(1, 1, 1);
bool grid = true;


class Window {
public:
    Window() {
        this->interval = 1000 / 60;		//60 FPS
        this->window_handle = -1;
    }
    int window_handle, interval;
    ivec2 size;
    float window_aspect;
} window;


void drawSpheres() {
    for (int i = 0; i < vw->getVertices()->size(); i++) {
        shared_ptr<vec4> vertex = vw->getVertices()->at(i).first;
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
 * grid underlying square.
 * necessary because mouse position is only correct detected while clicking on objects
 */
//TODO: make invisible
void drawGridPane() {
    glPushMatrix();
    glColor3f(gridPaneColor.r, gridPaneColor.g, gridPaneColor.b);
    glBegin(GL_POLYGON);
    glVertex3f(-GRIDLENGTH/2, -0.1, -GRIDLENGTH/2);
    glVertex3f(GRIDLENGTH/2, -0.1, -GRIDLENGTH/2);
    glVertex3f(GRIDLENGTH/2, -0.1, GRIDLENGTH/2);
    glVertex3f(-GRIDLENGTH/2, -0.1, GRIDLENGTH/2);
    glEnd();
    glPopMatrix();

}

void drawGrid() {
    int i;
    for (i = -GRIDLENGTH; i <= GRIDLENGTH; i++) {
        if (i <= 0) {
            glPushMatrix();
            glTranslatef(0, 0, i + GRIDLENGTH/2);
            glLineWidth(1);
            glBegin(GL_LINES);
            glColor3f(gridColor.r, gridColor.g, gridColor.b);
            glVertex3f(-GRIDLENGTH/2, 0, 0);
            glVertex3f(GRIDLENGTH/2, 0, 0);
            glEnd();
            glPopMatrix();
        }
        if (i >= 0) {
            glPushMatrix();
            glTranslatef(i - GRIDLENGTH/2, 0, 0);
            glRotatef(-90, 0, 1, 0);
            glLineWidth(1);
            glBegin(GL_LINES);
            glColor3f(gridColor.r, gridColor.g, gridColor.b);
            glVertex3f(-GRIDLENGTH/2, 0, 0);
            glVertex3f(GRIDLENGTH/2, 0, 0);
            glEnd();
            glPopMatrix();
        }

    }
}
void drawLines(){
    glPushMatrix();
    glLineWidth(4);
    glBegin(GL_LINES);
    glColor3f(gridColor.r, gridColor.g, gridColor.b);
    for (int i = 0; i < vecW->getEdges()->size(); i++) {
        glVertex3f(vecW->getEdges()->at(i).first->x, vecW->getEdges()->at(i).first->y, vecW->getEdges()->at(i).first->z);
        glVertex3f(vecW->getEdges()->at(i).second->x, vecW->getEdges()->at(i).second->y, vecW->getEdges()->at(i).second->z);
    }
    glEnd();
    glPopMatrix();
}

void display() {

    glEnable(GL_DEPTH_TEST);
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, window.size.x, window.size.y);

    mat4 model, view, projection;
    camera.Update();
    camera.GetMatricies(projection, view, model);

    mat4 mvp = projection* view * model;	//Compute the mvp matrix
    glLoadMatrixf(value_ptr(mvp));

    if(grid){
        drawGrid();
        drawGridPane();
    }
    drawSpheres();
    drawLines();
    glutSwapBuffers();
}
//Used when person drags mouse around
void CallBackMotionFunc(int x, int y) {
    camera.Move2D(x, y);
}
//Redraw based on fps set for window
void TimerFunc(int value) {
    if (window.window_handle != -1) {
        glutTimerFunc(window.interval, TimerFunc, value);
        glutPostRedisplay();
    }
}



//Invalidate the window handle when window is closed
void CloseFunc() {
    window.window_handle = -1;
}

//Resize the window and properly update the camera viewport
void ReshapeFunc(int w, int h) {
    if (h > 0) {
        window.size = ivec2(w, h);
        window.window_aspect = float(w) / float(h);
    }
    camera.SetViewport(0, 0, window.size.x, window.size.y);
}


void initOGLWidget(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDOWWIDTH, WIDOWHEIGHT);

    //Setup window and callbacks
    window.window_handle = glutCreateWindow("modeller");
    glutReshapeFunc(ReshapeFunc);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutMouseFunc(mouseClicks);
    glutMotionFunc(CallBackMotionFunc);
    glutTimerFunc(window.interval, TimerFunc, 0);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        cerr << "GLEW failed to initialize." << endl;
    }
    //Setup camera
    camera.SetMode(FREE);
    camera.SetPosition(vec3(20, 10, 10));
    camera.SetLookAt(vec3(0, 0, 0));
    camera.SetClipping(.1, 1000);
    camera.SetFOV(45);
    //Start the glut loop!
    glutMainLoop();

}



vec3 getWorldCoordinates(int x, int y) {
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

    //cout << objx << " , " << objy << " , " << objz << "\n";

    return vec3(objx, objy, objz);

}



















