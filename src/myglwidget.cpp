// myglwidget.cpp

#define GLEW_STATIC
#include "glew.c"

#include "myglwidget.h"

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

    char *model_path = (( char *)"../obj/teapot.obj");
    char *vertexshader_path = ( char *) "../shader/simpleShader.vert";
    char *fragmentshader_path = ( char *) "../shader/simpleShader.frag";

    qglClearColor(Qt::black);


    // to properly initialize all available OpenGL function pointers
    glewExperimental = GL_TRUE;
    // place this in initializeGL()
    glewInit();

    //glEnable(GL_COLOR_MATERIAL);
    //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    // create shader, prepare data for OpenGL
    trig.LoadFile(model_path);
    shader.Init(vertexshader_path, fragmentshader_path);
    setup_vertex_position_buffer_object();
    setup_vertex_uv_buffer_object();
    setup_vertex_normal_buffer_object(true);
    // set up camera and object transformation matrices
    //projectionMatrix = get_default_projectionMatrix();
    //viewMatrix = get_default_viewMatrix();
    //modelMatrix = get_default_modelMatrix();
    normalMatrix = get_default_normalMatrix();


    //Setup camera
    camera->SetMode(FREE);
    //camera->SetPosition(dimensions.getCameraPosition());
    camera->SetPosition(vec3(0, 100, 200));
    camera->SetLookAt(vec3(0, 0, 0));
    camera->SetClipping(.1, 1000);
    camera->SetFOV(45);
}

glm::mat3 MyGLWidget::get_default_normalMatrix(void) {
    return glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
}

glm::mat4 MyGLWidget::get_default_viewMatrix(void) {
    return glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -50.0f, -300.0f));
}
glm::mat4 MyGLWidget::get_default_modelMatrix(void) {
    return glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
}
glm::mat4 MyGLWidget::get_default_projectionMatrix(void) {
    return glm::ortho(-windowX * 0.5f,
                      windowX * 0.5f,
                      -windowY * 0.5f,
                      windowY * 0.5f,
                      -1.0f, 400.0f);
}


void MyGLWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);



    camera->Update();
    camera->GetMatricies(projectionMatrix, viewMatrix, modelMatrix);

    mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;    //Compute the mvp matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(value_ptr(mvp));
    glMatrixMode(GL_MODELVIEW);


    shader.Bind();

    // pass uniform variables to shader
    GLint projectionMatrix_location    = glGetUniformLocation(shader.ID(), "projectionMatrix");
    GLint viewMatrix_location          = glGetUniformLocation(shader.ID(), "viewMatrix");
    GLint modelMatrix_location         = glGetUniformLocation(shader.ID(), "modelMatrix");
    GLint normalMatrix_location        = glGetUniformLocation(shader.ID(), "normalMatrix");
    GLint materialAmbient_location     = glGetUniformLocation(shader.ID(), "materialAmbient");
    GLint materialDiffuse_location     = glGetUniformLocation(shader.ID(), "materialDiffuse");
    GLint materialSpecular_location    = glGetUniformLocation(shader.ID(), "materialSpecular");
    GLint lightPosition_location       = glGetUniformLocation(shader.ID(), "lightPosition");
    GLint lightAmbient_location        = glGetUniformLocation(shader.ID(), "lightAmbient");
    GLint lightDiffuse_location        = glGetUniformLocation(shader.ID(), "lightDiffuse");
    GLint lightSpecular_location       = glGetUniformLocation(shader.ID(), "lightSpecular");
    GLint lightGlobal_location         = glGetUniformLocation(shader.ID(), "lightGlobal");
    GLint materialShininess_location   = glGetUniformLocation(shader.ID(), "materialShininess");
    GLint constantAttenuation_location = glGetUniformLocation(shader.ID(), "constantAttenuation");
    GLint linearAttenuation_location   = glGetUniformLocation(shader.ID(), "linearAttenuation");
    GLint useTexture_location          = glGetUniformLocation(shader.ID(), "useTexture");
    glUniformMatrix4fv( projectionMatrix_location, 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniformMatrix4fv( viewMatrix_location,       1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv( modelMatrix_location,      1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix3fv( normalMatrix_location,     1, GL_FALSE, &normalMatrix[0][0]);
    glUniform3fv(       materialAmbient_location,  1, materialAmbient);
    glUniform3fv(       materialDiffuse_location,  1, materialDiffuse);
    glUniform3fv(       materialSpecular_location, 1, materialSpecular);
    glUniform3fv(       lightPosition_location,    1, lightPosition);
    glUniform3fv(       lightAmbient_location,     1, lightAmbient);
    glUniform3fv(       lightDiffuse_location,     1, lightDiffuse);
    glUniform3fv(       lightSpecular_location,    1, lightSpecular);
    glUniform3fv(       lightGlobal_location,      1, lightGlobal);
    glUniform1f(        materialShininess_location,   materialShininess);
    glUniform1f(        constantAttenuation_location, constantAttenuation);
    glUniform1f(        linearAttenuation_location,   linearAttenuation);
    glUniform1i(        useTexture_location,          0);

    /*
    // bind texture to shader
    GLint texture0_location = glGetAttribLocation(shader.ID(), "texture0");
    if (texture0_location != -1) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(texture0_location, 0);
    }
     */

    // bind vertex uv coordinates to shader
    GLint uv_location = glGetAttribLocation(shader.ID(), "vertex_uv");
    if (uv_location != -1) {
        glEnableVertexAttribArray(uv_location);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_uv_buffer);
        glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
    // bind vertex positions to shader
    GLint position_location = glGetAttribLocation(shader.ID(), "vertex_position");
    if (position_location != -1) {
        glEnableVertexAttribArray(position_location);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_position_buffer);
        glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    // bind vertex normals to shader
    GLint normal_location = glGetAttribLocation(shader.ID(), "vertex_normal");
    if (normal_location != -1) {
        glEnableVertexAttribArray(normal_location);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_normal_buffer);
        glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // draw the scene
    glDrawArrays(GL_TRIANGLES, 0, trig.VertexCount());
    glDisableVertexAttribArray(position_location);
    glDisableVertexAttribArray(uv_location);
    glDisableVertexAttribArray(normal_location);
    shader.Unbind();


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


void MyGLWidget::setup_vertex_position_buffer_object(void) {
    glGenBuffers(1, &vertex_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * trig.VertexCount(),
                 &trig.Vertices()[0], GL_STATIC_DRAW);
}
void MyGLWidget::setup_vertex_uv_buffer_object(void) {
    glGenBuffers(1, &vertex_uv_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_uv_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * trig.UVs().size(),
                 &trig.UVs()[0], GL_STATIC_DRAW);
}
void MyGLWidget::setup_vertex_normal_buffer_object(bool smoothed) {
    std::vector<glm::vec3> vertices = trig.Vertices();
    std::vector<glm::vec3> normals;
    if (smoothed) {
        // initialize map of normals to zero
        // note that we store readily hashable vector<double> types instead of
        // vec3s and convert between the two as required
        // ...avoids some of the pain using <map> without much C++ knowledge
        std::map< std::vector<double>, std::vector<double> > normal_map;
        for (int i = 0; i < vertices.size(); i++) {
            std::vector<double> zeros;
            zeros.push_back(0.0);
            zeros.push_back(0.0);
            zeros.push_back(0.0);
            normal_map[to_vector(vertices[i])] = zeros;
        }
        for (int i = 0; i < vertices.size(); i += 3) {
            // get vertices of the current triangle
            glm::vec3 v1 = vertices[i];
            glm::vec3 v2 = vertices[i + 1];
            glm::vec3 v3 = vertices[i + 2];
            std::vector<double> v1_key = to_vector(v1);
            std::vector<double> v2_key = to_vector(v2);
            std::vector<double> v3_key = to_vector(v3);
            // compute face normal
            glm::vec3 face_normal = glm::cross(v3 - v2, v1 - v2);
            // get the old vertex normal
            glm::vec3 v1_old = to_vec3(normal_map[v1_key]);
            glm::vec3 v2_old = to_vec3(normal_map[v2_key]);
            glm::vec3 v3_old = to_vec3(normal_map[v3_key]);
            // replace the old value with the new value
            normal_map.erase(v1_key);
            normal_map.erase(v2_key);
            normal_map.erase(v3_key);
            normal_map[v1_key] = to_vector(glm::normalize(v1_old + face_normal));
            normal_map[v2_key] = to_vector(glm::normalize(v2_old + face_normal));
            normal_map[v3_key] = to_vector(glm::normalize(v3_old + face_normal));
        }
        // convert the map of normals to a vector of normals
        for (int i = 0; i < vertices.size(); i++) {
            normals.push_back(to_vec3(normal_map[to_vector(vertices[i])]));
        }
    } else {
        for (int i = 0; i < vertices.size(); i += 3) {
            // get vertices of this triangle
            glm::vec3 v1 = vertices[i];
            glm::vec3 v2 = vertices[i + 1];
            glm::vec3 v3 = vertices[i + 2];
            // compute face normal
            glm::vec3 face_normal = glm::cross(v3 - v2, v1 - v2);
            normals.push_back(glm::normalize(face_normal));
            normals.push_back(glm::normalize(face_normal));
            normals.push_back(glm::normalize(face_normal));
        }
    }
    glGenBuffers(1, &vertex_normal_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
                 &normals[0], GL_STATIC_DRAW);
}


void MyGLWidget::draw() {





}
