// myglwidget.cpp

#define GLEW_STATIC

#include "glew.c"

#include "myglwidget.h"

#define _USE_MATH_DEFINES

#include <math.h>

using namespace cg;
using namespace glm;
using namespace std;


MyGLWidget::MyGLWidget(QWidget *parent)
        : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {

    camera = new Camera();

    QWidget::setFocusPolicy(Qt::ClickFocus);

    xRot = 0;
    yRot = 0;
    zRot = 0;

    backgroundColor = vec3(0, 0, 0);

}

MyGLWidget::~MyGLWidget() {
}

QSize MyGLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize MyGLWidget::sizeHint() const {
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle) {
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void MyGLWidget::setXRotation(int angle) {
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setYRotation(int angle) {
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setZRotation(int angle) {
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::initializeGL() {

    char *model_path = ((char *) "../obj/teapot.obj");
    char *vertexshader_path = (char *) "../shader/simpleShader.vert";
    char *fragmentshader_path = (char *) "../shader/simpleShader.frag";

    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1);

    glewExperimental = GL_TRUE;
    glewInit();


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_POINT_SMOOTH);


    //enable vertex radius in shader
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);


    //// create shader
    shader.Init(vertexshader_path, fragmentshader_path);

    //prepare data for OpenGL
    renderer.initRenderer(shader, model_path);



    //Setup camera
    camera->SetMode(FREE);
    //camera->SetPosition(dimensions.getCameraPosition());
    camera->SetPosition(vec3(0, 4, 7));
    camera->SetLookAt(vec3(0, 0, 0));
    camera->SetClipping(.1, 1000);
    camera->SetFOV(45);



}


void MyGLWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera->Update();
    camera->GetMatricies(projectionMatrix, viewMatrix, modelMatrix);

    mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;    //Compute the mvp matrix
    //glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(value_ptr(mvp));
    //glMatrixMode(GL_MODELVIEW);


    shader.Bind();

    shader.passUniformToShader(modelMatrix, viewMatrix, projectionMatrix, normalMatrix, camera->getCamera_position());

    renderer.render();

    shader.Unbind();


}


void MyGLWidget::resizeGL(int width, int height) {
    int side = qMin(width, height);
    glViewport(0, 0, width, height);
    camera->SetViewport(0, 0, width, height);

}

void MyGLWidget::mousePressEvent(QMouseEvent *event) {
    if (event->modifiers().testFlag(Qt::ControlModifier) && event->buttons() & Qt::LeftButton) {
        renderer.select(getWorldCoordinates(event->pos().x(), event->pos().y()));
    }else if(event->buttons() & Qt::RightButton){
        renderer.addVertex(getWorldCoordinates(event->pos().x(), event->pos().y()));
    }
    updateGL();
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        camera->Move2D(event->x(), event->y());
        updateGL();
    }

}

void MyGLWidget::keyPressEvent(QKeyEvent *event) {

    float moveStepSize = 0.01;

    switch (event->key()) {
        case Qt::Key_Escape:
            close();
            break;
        case Qt::Key_W:
            camera->Move(FORWARD);
            updateGL();
            break;
        case Qt::Key_A:
            camera->Move(LEFT);
            updateGL();
            break;
        case Qt::Key_S:
            camera->Move(BACK);
            updateGL();
            break;
        case Qt::Key_D:
            camera->Move(RIGHT);
            updateGL();
            break;
        case Qt::Key_Q:
            camera->Move(DOWN);
            updateGL();
            break;
        case Qt::Key_E:
            camera->Move(UP);
            updateGL();
            break;
        case Qt::Key_G:
            renderer.setDrawGrid(!renderer.isDrawGrid());
            updateGL();
            break;
        case Qt::Key_R:
            renderer.deleteSelectedVertices();
            updateGL();
            break;
        case Qt::Key_Left:
            renderer.moveSelected(glm::vec3(-moveStepSize, 0.0, 0.0));
            updateGL();
            break;
        case Qt::Key_Right:
            renderer.moveSelected(glm::vec3(moveStepSize, 0.0, 0.0));
            updateGL();
            break;
        case Qt::Key_Up:
            renderer.moveSelected(glm::vec3(0.0, 0.0, -moveStepSize));
            updateGL();
            break;
        case Qt::Key_Down:
            renderer.moveSelected(glm::vec3(0.0, 0.0, moveStepSize));
            updateGL();
            break;
        case Qt::Key_Minus:
            renderer.moveSelected(glm::vec3(0.0, -moveStepSize, 0.0));
            updateGL();
            break;
        case Qt::Key_Plus:
            renderer.moveSelected(glm::vec3(0.0, moveStepSize, 0.0));
            updateGL();
            break;
        default:
            break;
    }


}


vec3 MyGLWidget::getWorldCoordinates(int x, int y) {

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


    return vec3(objx, objy, objz);


}



