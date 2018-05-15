//
// Created by Johannes on 30.04.2018.
//


#include "OGLWidget.h"
#include "OGLWidgetUI.h"



using namespace std;
using namespace glm;

class Window {
public:
    Window() {
        this->interval = 1000 / 60;        //60 FPS
        this->window_handle = -1;
    }

    int window_handle, interval;
    glm::ivec2 size;
    float window_aspect;
} window;

cg::OGLWidget* currentInstance;

cg::OGLWidgetUI* currentInstanceUI;

extern "C"
void displayCallback()
{
    ::currentInstance->display();
}
void timerFuncCallback(int value){
    ::currentInstance->timerFunc(value);
}
void motionFuncCallback(int x, int y){
    ::currentInstance->motionFunc(x, y);
}
void ReshapeFuncCallback(int w, int h){
    ::currentInstance->ReshapeFunc(w, h);
}
void keyboardCallback(unsigned char key, int x, int y){
    ::currentInstanceUI->keyboard(key, x, y);
}
void mouseClicksCallback(int button, int state, int x, int y){
    ::currentInstanceUI->mouseClicks(button, state, x, y);
}
void specialKeyboardCallback(int key, int x, int y){
    ::currentInstanceUI->specialKeyboard(key, x, y);
}







namespace cg {



    OGLWidget::OGLWidget(const OBJDimensions &dimensions){

        this->dimensions = dimensions;
        this->dimensions.initData();

        backgroundColor = vec3(0, 0, 0);
        gridPaneColor = vec3(0.2, 0.2, 0.2);
        selectedSphereColor = vec3(1, 0, 0);
        sphereColor = vec3(1, 1, 1);
        gridColor = vec3(1, 1, 1);
        faceColor = vec3(0.2, 0.4, 0.4);


        grid = true;

        camera = new Camera();

    }





    void OGLWidget::drawSpheres() {

        for (int i = 0; i < wrapperPtr->getVerts().size(); i++) {
            shared_ptr<HE_vert> vertex = wrapperPtr->getVerts().at(i);
            glPushMatrix();
            if (wrapperPtr->getVerts().at(i)->selected) {
                glColor3f(selectedSphereColor.r, selectedSphereColor.g, selectedSphereColor.b);
            } else {
                glColor3f(sphereColor.r, sphereColor.g, sphereColor.b);

            }

            glTranslatef(vertex->pos.x, vertex->pos.y, vertex->pos.z);
            glutSolidSphere(dimensions.getSphereRadius(), dimensions.getSphereSlices(), dimensions.getSphereStacks());
            glPopMatrix();
        }


    }

/**
 * grid underlying square.
 * necessary because mouse position is only correct detected while clicking on objects
 */
//TODO: make invisible
    void OGLWidget::drawGridPane() {
        float grid_lenght = dimensions.getGridSize();
        glm::vec3 gridPosition = dimensions.getGridPosition();

        glPushMatrix();
        glColor3f(gridPaneColor.r, gridPaneColor.g, gridPaneColor.b);
        glBegin(GL_POLYGON);
        glVertex3f(-grid_lenght / 2, gridPosition.y - 0.1, -grid_lenght / 2);
        glVertex3f(grid_lenght / 2, gridPosition.y - 0.1, -grid_lenght / 2);
        glVertex3f(grid_lenght / 2, gridPosition.y - 0.1, grid_lenght / 2);
        glVertex3f(-grid_lenght / 2, gridPosition.y - 0.1, grid_lenght / 2);
        glEnd();
        glPopMatrix();

    }

    void OGLWidget::drawGrid() {

        float grid_lenght = dimensions.getGridSize();

        //TODO integrate gridPosition
        glm::vec3 gridPosition = dimensions.getGridPosition();
        int i;
        for (i = -grid_lenght; i <= grid_lenght; i++) {
            if (i <= 0) {
                glPushMatrix();
                glTranslatef(0, 0, i + grid_lenght / 2);
                glLineWidth(1);
                glBegin(GL_LINES);
                glColor3f(gridColor.r, gridColor.g, gridColor.b);
                glVertex3f(-grid_lenght / 2, gridPosition.y, 0);
                glVertex3f(grid_lenght / 2, gridPosition.y, 0);
                glEnd();
                glPopMatrix();
            }

            if (i >= 0) {
                glPushMatrix();
                glTranslatef(i - grid_lenght / 2, 0, 0);
                glRotatef(-90, 0, 1, 0);
                glLineWidth(1);
                glBegin(GL_LINES);
                glColor3f(gridColor.r, gridColor.g, gridColor.b);
                glVertex3f(-grid_lenght / 2, gridPosition.y, 0);
                glVertex3f(grid_lenght / 2, gridPosition.y, 0);
                glEnd();
                glPopMatrix();

            }

        }


    }



    void OGLWidget::drawFaces() {

        glFrontFace(GL_CCW);

        for (FacePointer fp : wrapperPtr->getFaces()) {
            EdgePointer start = fp->edge;
            EdgePointer curr = start;
            if (start != nullptr) {
                glPushMatrix();
                glBegin(GL_POLYGON);
                //glm::vec3 faceNormal = fp->faceNormal;
                //glNormal3f(faceNormal.x, faceNormal.y, faceNormal.z);
                glColor3f(faceColor.r, faceColor.g, faceColor.b);

                do {
                    glm::vec3 vertNormal = curr->vert->vertNormal;
                    glNormal3f(vertNormal.x, vertNormal.y, vertNormal.z);
                    glVertex3f(curr->vert->pos.x, curr->vert->pos.y,
                               curr->vert->pos.z);
                    curr = curr->next;
                } while (curr != start);

                glEnd();
                glPopMatrix();
            }

        }




    }

    void OGLWidget::drawNormals() {

        for(VertPointer vp : wrapperPtr->getVerts()){
            glPushMatrix();
            glBegin(GL_LINES);
            glLineWidth(2);
            glColor3f(gridColor.r, gridColor.g, gridColor.b);
            glVertex3f(vp->pos.x, vp->pos.y, vp->pos.z);
            glVertex3f(vp->pos.x + vp->vertNormal.x, vp->pos.y + vp->vertNormal.y, vp->pos.z + vp->vertNormal.z);
            glEnd();
            glPopMatrix();
        }


/*
        for(FacePointer fpp : wrapperPtr->getFaces()){
            VertPointer vvp = fpp->edge->vert;
            glPushMatrix();
            glBegin(GL_LINES);
            glLineWidth(2);
            glColor3f(1.0, 0.0, 0.0);
            glVertex3f(vvp->pos.x, vvp->pos.y, vvp->pos.z);
            glVertex3f(vvp->pos.x + fpp->faceNormal.x, vvp->pos.y + fpp->faceNormal.y, vvp->pos.z + fpp->faceNormal.z);
            glEnd();
            glPopMatrix();
        }
        */
    }

    void OGLWidget::display() {

        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, window.size.x, window.size.y);

        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


        mat4 model, view, projection;
        camera->Update();
        camera->GetMatricies(projection, view, model);

        mat4 mvp = projection * view * model;    //Compute the mvp matrix
        glLoadMatrixf(value_ptr(mvp));


        if (grid) {
            glDisable(GL_LIGHT0);
            drawGrid();
            drawGridPane();
            glEnable(GL_LIGHT0);

        }
        drawSpheres();
        drawFaces();

        //drawNormals();

        glutSwapBuffers();
    }

//Used when person drags mouse around
    void OGLWidget::motionFunc(int x, int y) {
        camera->Move2D(x, y);
    }

//Redraw based on fps set for window
    void OGLWidget::timerFunc(int value) {
        if (window.window_handle != -1) {
            glutTimerFunc(window.interval, ::timerFuncCallback, value);
            glutPostRedisplay();
        }
    }


//Invalidate the window handle when window is closed
    void OGLWidget::CloseFunc() {
        window.window_handle = -1;
    }

//Resize the window and properly update the camera viewport
    void OGLWidget::ReshapeFunc(int w, int h) {
        if (h > 0) {
            window.size = ivec2(w, h);
            window.window_aspect = float(w) / float(h);
        }
        camera->SetViewport(0, 0, window.size.x, window.size.y);
    }



    void OGLWidget::initOGLWidget(int argc, char **argv, const std::shared_ptr<HE_Wrapper> wrapper) {

        wrapperPtr = wrapper;

        ::currentInstance = this;
        ::currentInstanceUI = new OGLWidgetUI(this);


        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
        glutInitWindowSize(WIDOWWIDTH, WIDOWHEIGHT);

        //Setup window and callbacks
        window.window_handle = glutCreateWindow("modeller");
        glutReshapeFunc(::ReshapeFuncCallback);
        glutDisplayFunc(::displayCallback);

        glutKeyboardFunc(::keyboardCallback);
        glutSpecialFunc(::specialKeyboardCallback);
        glutMouseFunc(::mouseClicksCallback);

        glutMotionFunc(::motionFuncCallback);
        glutTimerFunc(window.interval, ::timerFuncCallback, 0);

        glewExperimental = GL_TRUE;

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);



        glShadeModel(GL_SMOOTH);

        // GL_LIGHT0: the white light emitting light source
        // Create light components for GL_LIGHT0
        float ambientLight0[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        float diffuseLight0[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        float specularLight0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
        glm::vec3 lightPosition = dimensions.getLightPosition();
        GLfloat position0[] = {lightPosition.x, lightPosition.y, lightPosition.z, 1.0f};
        // Assign created components to GL_LIGHT0
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
        glLightfv(GL_LIGHT0, GL_POSITION, position0);



        if (glewInit() != GLEW_OK) {
            cerr << "GLEW failed to initialize." << endl;
        }

        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

        //Setup camera
        camera->SetMode(FREE);
        camera->SetPosition(vec3(20, 10, 10));
        camera->SetLookAt(vec3(0, 0, 0));
        camera->SetClipping(.1, 1000);
        camera->SetFOV(45);

        //Start the glut loop!
        glutMainLoop();

    }


    vec3 OGLWidget::getWorldCoordinates(int x, int y) {
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



    const shared_ptr<HE_Wrapper> OGLWidget::getWrapperPtr() const {
        return wrapperPtr;
    }



    Camera *OGLWidget::getCamera() const {
        return camera;
    }


    bool OGLWidget::isGrid() const {
        return grid;
    }

    void OGLWidget::setGrid(bool grid) {
        OGLWidget::grid = grid;
    }

    const OBJDimensions &OGLWidget::getDimensions() const {
        return dimensions;
    }


}












