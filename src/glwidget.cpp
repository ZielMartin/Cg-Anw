// myglwidget.cpp

#include <QVector3D>
#include "GL/glew.h"
#include <QMatrix4x4>

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

    backgroundColor = vec3(0.2f, 0.2f, 0.2f);

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
    viewport = QVector4D(0, 0, width, height);
    camera->SetViewport(0, 0, width, height);
    camera->Update();

}

void GLWidget::mousePressEvent(QMouseEvent *event) {

    if (event->modifiers().testFlag(Qt::ControlModifier) && event->buttons() & Qt::LeftButton) {

        renderer.select(getWorldCoordinates(event->pos().x(), event->pos().y()));

        //checkbox represents first selectedf half-edge
        if(renderer.getMeshWrapper().getSelectedHalfEdges().size() >= 1){
            emit valueChanged(renderer.getMeshWrapper().getMesh().property(renderer.getMeshWrapper().getMesh().sharpedge, renderer.getMeshWrapper().getSelectedHalfEdges().at(0)));
        }

        updateGL();
    } else if (event->modifiers().testFlag(Qt::ShiftModifier) && event->buttons() & Qt::LeftButton) {
        QRect viewp(viewport.x(), viewport.y(), viewport.z(), viewport.w());

        QMatrix4x4 qviewMatrix = { viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0], viewMatrix[3][0],
                                   viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1], viewMatrix[3][1],
                                   viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2], viewMatrix[3][2],
                                   viewMatrix[0][3], viewMatrix[1][3], viewMatrix[2][3], viewMatrix[3][3],};
        QMatrix4x4 qprojectionMatrix = { projectionMatrix[0][0], projectionMatrix[1][0], projectionMatrix[2][0], projectionMatrix[3][0],
                                         projectionMatrix[0][1], projectionMatrix[1][1], projectionMatrix[2][1], projectionMatrix[3][1],
                                         projectionMatrix[0][2], projectionMatrix[1][2], projectionMatrix[2][2], projectionMatrix[3][2],
                                         projectionMatrix[0][3], projectionMatrix[1][3], projectionMatrix[2][3], projectionMatrix[3][3],};

        QVector3D begin = QVector3D(event->pos().x(), height() - 1 - event->pos().y(), 0.0f).unproject(qviewMatrix, qprojectionMatrix, viewp);
        QVector3D end = QVector3D(event->pos().x(), height() - 1 - event->pos().y(), 1.0f).unproject(qviewMatrix, qprojectionMatrix, viewp);

        // Create ray.
        QVector3D origin = begin;
        QVector3D direction = (end - begin).normalized();

        intersect(origin, direction);

        emit repaint();

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

void GLWidget::intersect(const QVector3D& origin, const QVector3D& direction) {
    float minimum = std::numeric_limits<float>::max();
    HE_MESH::HalfedgeHandle closest;
    HE_MESH mesh = renderer.getMeshWrapper().getMesh();
    HE_MESH::HalfedgeIter hiter = renderer.getMeshWrapper().getMesh().halfedges_begin();
    HE_MESH::HalfedgeIter end = renderer.getMeshWrapper().getMesh().halfedges_end();

    for (; hiter != end; ++hiter) {
        HE_MESH::Point start = mesh.point(mesh.from_vertex_handle(hiter));
        HE_MESH::Point end = mesh.point(mesh.from_vertex_handle(mesh.next_halfedge_handle(hiter)));

        const int n = 32;

        for (int i = 0; i < n + 1; i++)
        {
            float t = (float) i / n;
            HE_MESH::Point mid = (1.0f - t) * start + t * end;
            QVector3D midd = {mid[0], mid[1], mid[2]};
            float distance = midd.distanceToLine(origin, direction);

            if (minimum > distance)
            {
                minimum = distance;
                closest = *hiter;
            }
        }
    }
    mesh.property(mesh.he_selected, closest) = true;
    mesh.property(mesh.he_selected, mesh.opposite_halfedge_handle(closest)) = true;
    selectedEdge = closest;
    emit valueChanged(mesh.property(mesh.sharpedge, closest));
}


void GLWidget::saveOBJ(char *file) {
    renderer.getMeshWrapper().writeMesh(file);

}

void GLWidget::applySmoothedVertices(int interpolationValue) {
    renderer.getMeshWrapper().applySmoothedVertices(interpolationValue);
    renderer.recreateMesh();
    updateGL();

}

std::vector<std::pair<std::string, int>> GLWidget::meshInfo() {
    return renderer.getMeshWrapper().getMeshInfo();
};


void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        camera->Move2D(event->x(), event->y());
        updateGL();
    }

}

void GLWidget::keyPressEvent(QKeyEvent *event) {

    float moveStepSize = 0.03;

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
        case Qt::Key_Y:
            renderer.renderLimitNormals = renderer.renderLimitNormals == true ? false : true;
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
        case Qt::Key_T:
            renderer.getMeshWrapper().test();
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


void GLWidget::setVertexWeight(float weight) {
    renderer.getMeshWrapper().setVertexWeightAllSelected(weight);
}

void GLWidget::setLimitRules(bool LR) {
    renderer.getMeshWrapper().limit = LR;
}

void GLWidget::setSharpEdge(bool sharp) {
        if (selectedEdge.is_valid()) {
            HE_MESH &mesh = renderer.getMeshWrapper().getMesh();
            mesh.property(mesh.sharpedge, selectedEdge) = sharp;
            mesh.property(mesh.sharpedge, mesh.opposite_halfedge_handle(selectedEdge)) = sharp;
        }
    renderer.getMeshWrapper().setSelectedEdgesSharp(sharp);
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



