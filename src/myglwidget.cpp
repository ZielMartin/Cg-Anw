// myglwidget.cpp


#include <QtWidgets>
#include <QtOpenGL>
#include <include/glextload.h>
#include "myglwidget.h"
//#include "glextload.h"

using namespace cg;
using namespace glm;
using namespace std;

const char* vertex_shader =
        "attribute vec3 aCoords;"
        "attribute vec3 aColor;"
        "uniform mat4 umvMat;"
        "uniform mat4 upMat;"
        "varying vec3 vColor;"
        "void main () {"
        "gl_Position = upMat * umvMat * vec4(aCoords, 1.0);"
        "vColor = aColor;"
        "}";

const char* fragment_shader =
        "varying vec3 vColor;"
        "void main () {"
        "gl_FragColor = vec4 (vColor, 1.0);"
        "}";


GLuint* vboIds = NULL;
GLuint* vaoIds = NULL;
GLuint program = 0;
GLuint VERTEX_ATTR_COORDS = 1;
GLuint VERTEX_ATTR_COLOR = 2;

const int nCoordsComponents = 3;
const int nColorComponents = 3;
const int nLines = 3;
const int nVerticesPerLine = 2;
const int nFaces = 6;
const int nVerticesPerFace = 3;


float av[] = { 0.0, 0.0, 0.0,    // origin
               2.0, 0.0, 0.0,    // x-axis
               0.0, 2.0, 0.0,    // y-axis
               0.0, 0.0, 2.0 };  // z-axis

GLubyte avi[] = { 0, 1,
                  0, 2,
                  0, 3 };

float ac[] = { 1.0, 0.0, 0.0,    // red   x-axis
               0.0, 1.0, 0.0,    // green y-axis
               0.0, 0.0, 1.0 };  // blue  z-axis

GLubyte aci[] = { 0, 0,
                  1, 1,
                  2, 2 };

float ave[nLines*nVerticesPerLine*nCoordsComponents];
void expandAxesVertices()
{
    for (int i=0; i<6; i++)
    {
        ave[i*3+0] = av[avi[i]*3+0];
        ave[i*3+1] = av[avi[i]*3+1];
        ave[i*3+2] = av[avi[i]*3+2];
    }
}

float ace[nLines*nVerticesPerLine*nColorComponents];
void expandAxesColors()
{
    for (int i=0; i<6; i++)
    {
        ace[i*3+0] = ac[aci[i]*3+0];
        ace[i*3+1] = ac[aci[i]*3+1];
        ace[i*3+2] = ac[aci[i]*3+2];
    }
}

// =========== Pyramid Data =================================================

//  (3,4,5)          (6,7,8)
//     1----------------2
//     | \            / |
//     |   \        /   |
//     |     \    /     |
//     |        4       | (12,13,14)
//     |     /    \     |
//     |   /        \   |
//     | /            \ |
//     0 ---------------3
//  (0,1,2)          (9,10,11)

float pv[] = { 0.5, 0.5, 0.5,    // 0
               0.5, 1.5, 0.5,    // 1
               1.5, 1.5, 0.5,    // 2
               1.5, 0.5, 0.5,    // 3
               1.0, 1.0, 1.5 };  // 4

GLubyte pvi[] = {0, 1, 2,
                 2, 3, 0,
                 0, 3, 4,
                 3, 2, 4,
                 2, 1, 4,
                 1, 0, 4};

float pve[nFaces*nVerticesPerFace*nCoordsComponents];
void expandPyramidVertices()
{
    for (int i=0; i<nFaces; i++)
    {
        for (int j=0; j<nVerticesPerFace; j++)
        {
            for (int k=0; k<nCoordsComponents; k++)
            {
                pve[(i*3+j)*3+k] = pv[pvi[i*3+j]*3+k];
            }
        }
    }
}

float pc[] = { 0.3f, 0.30f, 0.3f,
               1.0f, 0.70f, 0.0f,
               1.0f, 0.62f, 0.0f,
               1.0f, 0.40f, 0.0f,
               1.0f, 0.48f, 0.0f};

GLubyte pci[] = { 0, 0, 0,
                  0, 0, 0,
                  1, 1, 1,
                  2, 2, 2,
                  3, 3, 3,
                  4, 4, 4 };

float pce[nFaces*nVerticesPerFace*nColorComponents];
void expandPyramidColors()
{
    for (int i=0; i<nFaces; i++)
    {
        for (int j=0; j<nVerticesPerFace; j++)
        {
            for (int k=0; k<nColorComponents; k++)
            {
                pce[(i*3+j)*3+k] = pc[pci[i*3+j]*3+k];
            }
        }
    }
}

float n[nFaces*nVerticesPerFace*nCoordsComponents];

// ===========================================================================

bool double_equal(double a, double b)
{
    if (fabs(a-b) < 1e-3)
        return true;
    return false;
}

void MyGLWidget::initShaders()
{
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource (vs, 1, &vertex_shader, NULL);
    glCompileShader (vs);

    GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fs, 1, &fragment_shader, NULL);
    glCompileShader (fs);

    program = glCreateProgram();
    glAttachShader (program, fs);
    glAttachShader (program, vs);

    glBindAttribLocation(program, VERTEX_ATTR_COORDS, "aCoords");
    glBindAttribLocation(program, VERTEX_ATTR_COLOR, "aColor");

    glLinkProgram (program);

    glUseProgram (program);
}





MyGLWidget::MyGLWidget(QWidget *parent)
        : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {

    camera = new Camera();

    QWidget::setFocusPolicy(Qt::ClickFocus);

    xRot = 0;
    yRot = 0;
    zRot = 0;
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
    qglClearColor(Qt::black);

    //glEnable(GL_COLOR_MATERIAL);
    //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    static GLfloat lightPosition[4] = {0, 10, 0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);


    expandAxesVertices();
    expandAxesColors();
    expandPyramidVertices();
    expandPyramidColors();



    //Setup camera
    camera->SetMode(FREE);
    //camera->SetPosition(dimensions.getCameraPosition());
    camera->SetPosition(vec3(0, 0, 2));
    camera->SetLookAt(vec3(0, 0, 0));
    camera->SetClipping(.1, 1000);
    camera->SetFOV(45);
}

void MyGLWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);


    mat4 model, view, projection;
    camera->Update();
    camera->GetMatricies(projection, view, model);

    mat4 mvp = projection * view * model;    //Compute the mvp matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(value_ptr(mvp));
    glMatrixMode(GL_MODELVIEW);


    draw();
}

void MyGLWidget::resizeGL(int width, int height) {
    int side = qMin(width, height);
    glViewport(0, 0, width, height);
    camera->SetViewport(0, 0, width, height);

}

void MyGLWidget::mousePressEvent(QMouseEvent *event) {
    camera->SetPos(event);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        //camera->SetPos(event);
        camera->Move2D(event->x(), event->y());
        updateGL();
    }

}

void MyGLWidget::keyPressEvent(QKeyEvent *event) {

    switch(event->key()){
        case Qt::Key_Escape:
            close();
            break;
        case Qt::Key_W:
            camera->Move(FORWARD);
            break;
        case Qt::Key_A:
            camera->Move(LEFT);
            break;
        case Qt::Key_S:
            camera->Move(BACK);
            break;
        case Qt::Key_D:
            camera->Move(RIGHT);
            break;
        case Qt::Key_Q:
            camera->Move(DOWN);
            break;
        case Qt::Key_E:
            camera->Move(UP);
            break;
        default:
            break;
    }
    updateGL();


}

void MyGLWidget::draw() {


    drawShaderWithVertexArrayObject();


}

void MyGLWidget::defineVAO(){
    vaoIds = new GLuint[2];
    glGenVertexArrays(2, vaoIds);

    vboIds = new GLuint[4];
    glGenBuffers(4, vboIds);

    GLint vertexAttribCoords = glGetAttribLocation(program, "aCoords");
    GLint vertexAttribColor = glGetAttribLocation(program, "aColor");

    // Bind VAO (set current) to define axes data
    glBindVertexArray(vaoIds[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);  // coordinates
    glBufferData(GL_ARRAY_BUFFER, sizeof(ave), ave, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexAttribCoords, nCoordsComponents, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexAttribCoords);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);  // color
    glBufferData(GL_ARRAY_BUFFER, sizeof(ace), ace, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexAttribColor, nColorComponents, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexAttribColor);

    // Bind VAO (set current) to define pyramid data
    glBindVertexArray(vaoIds[1]);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[2]);  // coordinates
    glBufferData(GL_ARRAY_BUFFER, sizeof(pve), pve, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexAttribCoords, nCoordsComponents, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexAttribCoords);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[3]);  // color
    glBufferData(GL_ARRAY_BUFFER, sizeof(pce), pce, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexAttribColor, nColorComponents, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexAttribColor);

    // Disable VAO
    glBindVertexArray(0);
}

void MyGLWidget::drawShaderWithVertexArrayObject()
{
    LoadGLExtensions();

    initShaders();
    defineVAO();

    // Get the variables from the shader to which data will be passed
    GLint mvloc = glGetUniformLocation(program, "umvMat");
    GLint ploc = glGetUniformLocation(program, "upMat");

    // Pass the model-view matrix to the shader
    GLfloat mvMat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mvMat);
    glUniformMatrix4fv(mvloc, 1, false, mvMat);

    // Pass the projection matrix to the shader
    GLfloat pMat[16];
    glGetFloatv(GL_PROJECTION_MATRIX, pMat);
    glUniformMatrix4fv(ploc, 1, false, pMat);

    // Enable VAO to set axes data
    glBindVertexArray(vaoIds[0]);

    // Draw axes
    glDrawArrays(GL_LINES, 0, nLines*nVerticesPerLine);

    // Enable VAO to set pyramid data
    glBindVertexArray(vaoIds[1]);

    // Draw pyramid
    glDrawArrays(GL_TRIANGLES, 0, nFaces*nVerticesPerFace);

    // Disable VAO
    glBindVertexArray(0);
}