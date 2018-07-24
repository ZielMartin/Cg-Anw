// myglwidget.cpp

#include "GL/glew.h"

#include "glwidget.h"

#define _USE_MATH_DEFINES

#include <math.h>
#include <QtWidgets/QInputDialog>

using namespace cg;
using namespace glm;
using namespace std;


GLWidget::GLWidget(QWidget *parent, QGLFormat format)
        : QGLWidget(format, parent) {


    camera = new Camera();

    QWidget::setFocusPolicy(Qt::ClickFocus);

    backgroundColor = vec3(0, 0, 0);

}

GLWidget::~GLWidget() {
}

QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
    return QSize(400, 400);
}

void GLWidget::initializeGL() {


    //char *vertexshader_path = (char *) "../shader/simpleShader.vert";
    //char *fragmentshader_path = (char *) "../shader/simpleShader.frag";

    char *vertexshader_path = (char *) "shader/phongShader.vert";
    char *fragmentshader_path = (char *) "shader/phongShader.frag";

    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1);


    glewExperimental = GL_TRUE;
    glewInit();


    glEnable(GL_DEPTH_TEST);


    //enable point size in shader
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);


    //// create shader
    shader.Init(vertexshader_path, fragmentshader_path);
    openMesh(nullptr);



    //Setup camera
    camera->SetMode(FREE);
    //camera->SetPosition(dimensions.getCameraPosition());
    camera->SetPosition(vec3(0, 4, 7));
    camera->SetLookAt(vec3(0, 0, 0));
    camera->SetClipping(.1, 1000);
    camera->SetFOV(45);




}

void GLWidget::openMesh(char *model_path) {
    renderer.initRenderer(shader, model_path);
}


void GLWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera->GetMatricies(projectionMatrix, viewMatrix, modelMatrix);

    mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;    //Compute the mvp matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(value_ptr(mvp));
    glMatrixMode(GL_MODELVIEW);


    shader.Bind();

    shader.passUniformToShader(modelMatrix, viewMatrix, projectionMatrix, normalMatrix, camera->getCamera_position());

    renderer.render();

    shader.Unbind();


}


void GLWidget::resizeGL(int width, int height) {
    int side = qMin(width, height);
    glViewport(0, 0, width, height);
    camera->SetViewport(0, 0, width, height);
    camera->Update();

}

void GLWidget::mousePressEvent(QMouseEvent *event) {

    if (event->modifiers().testFlag(Qt::ControlModifier) && event->buttons() & Qt::LeftButton) {

        renderer.select(getWorldCoordinates(event->pos().x(), event->pos().y()));

        updateGL();
    } else if (event->buttons() & Qt::RightButton) {
        glm::vec3 worldPos = getWorldCoordinates(event->pos().x(), event->pos().y());
        renderer.getMeshWrapper().addVertex(worldPos);

        renderer.getMeshWrapper().deselectAll();

        renderer.recreateMesh();

        renderer.select(worldPos);
        updateGL();
    }


}

void GLWidget::saveOBJ(char *file){
    renderer.getMeshWrapper().writeMesh(file);

}

void GLWidget::applySmoothedVertices(int interpolationValue){
    renderer.getMeshWrapper().applySmoothedVertices(interpolationValue);
    renderer.recreateMesh();
    updateGL();

}

std::vector<std::pair<std::string, int>> GLWidget::meshInfo(){
    return renderer.getMeshWrapper().getMeshInfo();
};


void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        camera->Move2D(event->x(), event->y());
        updateGL();
    }

}

void GLWidget::keyPressEvent(QKeyEvent *event) {

    float moveStepSize = 0.01;

    if (event->modifiers().testFlag(Qt::ControlModifier) && event->key() == Qt::Key_Z) {
        renderer.getMeshWrapper().undo();
        renderer.recreateMesh();
        updateGL();
    }

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
            renderer.setRenderGrid(!renderer.isRenderGrid());
            updateGL();
            break;
        case Qt::Key_P:
            renderer.setRenderPoints(!renderer.isRenderPoints());
            updateGL();
            break;
        case Qt::Key_K:
            renderer.setRenderLines(!renderer.isRenderLines());
            updateGL();
            break;
        case Qt::Key_R:
            renderer.getMeshWrapper().deleteSelectedVertices();
            renderer.updateMesh(true);
            updateGL();
            break;
        case Qt::Key_F:
            renderer.getMeshWrapper().makeSelectedFace();
            renderer.getMeshWrapper().deselectAll();
            renderer.recreateMesh();
            updateGL();
            break;
        case Qt::Key_C:
            renderer.getMeshWrapper().subdivision();
            renderer.getMeshWrapper().deselectAll();
            renderer.recreateMesh();
            updateGL();
            break;
        case Qt::Key_L:
            renderer.getMeshWrapper().smoothMesh(true);
            renderer.getMeshWrapper().applySmoothedVertices(99);
            renderer.getMeshWrapper().deselectAll();
            renderer.recreateMesh();
            updateGL();
            break;
        case Qt::Key_Left:
            renderer.getMeshWrapper().moveSelectedVertices(glm::vec3(-moveStepSize, 0.0, 0.0));
            renderer.updateMesh(false);
            updateGL();
            break;
        case Qt::Key_Right:
            renderer.getMeshWrapper().moveSelectedVertices(glm::vec3(moveStepSize, 0.0, 0.0));
            renderer.updateMesh(false);
            updateGL();
            break;
        case Qt::Key_Up:
            renderer.getMeshWrapper().moveSelectedVertices(glm::vec3(0.0, 0.0, -moveStepSize));
            renderer.updateMesh(false);
            updateGL();
            break;
        case Qt::Key_Down:
            renderer.getMeshWrapper().moveSelectedVertices(glm::vec3(0.0, 0.0, moveStepSize));
            renderer.updateMesh(false);
            updateGL();
            break;
        case Qt::Key_Minus:
            renderer.getMeshWrapper().moveSelectedVertices(glm::vec3(0.0, -moveStepSize, 0.0));
            renderer.updateMesh(false);
            updateGL();
            break;
        case Qt::Key_Plus:
            renderer.getMeshWrapper().moveSelectedVertices(glm::vec3(0.0, moveStepSize, 0.0));
            renderer.updateMesh(false);
            updateGL();
            break;
        default:
            break;
    }


}


void GLWidget::setVertexWeight(float weight){
    renderer.getMeshWrapper().setVertexWeightAllSelected(weight);
}

void GLWidget::setLimitRules(bool LR){
    renderer.getMeshWrapper().limit = LR;
}

#define NORMALMATRIX {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0}
vec3 GLWidget::getWorldCoordinates(int x, int y) {

    GLdouble obj[3];
    GLint viewport[4];
    GLdouble model[16] = NORMALMATRIX;
    GLdouble *viewProjection = this->camera->viewProjection;
    GLfloat winX, winY, winZ;
    glGetIntegerv(GL_VIEWPORT, viewport);
    winX = (float) x;
    winY = (float) viewport[3] - (float) y;
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    gluUnProject(winX, winY, winZ, model, viewProjection, viewport, obj, obj + 1, obj + 2);

    return vec3(obj[0], obj[1], obj[2]);


}



