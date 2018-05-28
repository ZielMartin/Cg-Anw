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

    backgroundColor = vec3(0, 0, 0);
    gridPaneColor = vec3(0, 0, 0);
    selectedPointsColor = vec3(1, 0, 0);
    pointsColor = vec3(0.7, 0.7, 0.7);
    gridColor = vec3(1, 1, 1);
    faceColor = vec3(0.2, 0.4, 0.4);

    drawGrid = true;
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

    // to properly initialize all available OpenGL function pointers
    glewExperimental = GL_TRUE;
    // place this in initializeGL()
    glewInit();


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    //// create shader
    shader.Init(vertexshader_path, fragmentshader_path);

    //prepare data for OpenGL
    initMesh(model_path);
    initGrid();
    initGridUnderlayingPane();



    //Setup camera
    camera->SetMode(FREE);
    //camera->SetPosition(dimensions.getCameraPosition());
    camera->SetPosition(vec3(0, 100, 200));
    camera->SetLookAt(vec3(0, 0, 0));
    camera->SetClipping(.1, 1000);
    camera->SetFOV(45);
}

void MyGLWidget::initGrid() {
    vec3 color = gridColor;
    float grid_lenght = 200;//dimensions.getGridSize();
    glm::vec3 gridPosition = glm::vec3(0, 0, 0); //dimensions.getGridPosition();

    float stepsize = grid_lenght / 50;
    float i;
    for (i = -grid_lenght; i <= grid_lenght; i += stepsize) {
        if (i <= 0) {
            grid.vertices.push_back(glm::vec3(gridPosition.x - grid_lenght / 2, gridPosition.y,
                                              gridPosition.z + (i + grid_lenght / 2)));
            grid.vertices.push_back(glm::vec3(gridPosition.x + grid_lenght / 2, gridPosition.y,
                                              gridPosition.z + (i + grid_lenght / 2)));

            grid.colors.push_back(color);
            grid.colors.push_back(color);

        }

        if (i >= 0) {

            //glRotatef(-90, 0, 1, 0);

            glm::vec3 vert1 = glm::vec3((gridPosition.x - grid_lenght / 2), gridPosition.y, gridPosition.z);
            glm::vec3 vert2 = glm::vec3((gridPosition.x + grid_lenght / 2), gridPosition.y, gridPosition.z);

            //1,5708
            GLfloat tempX = vert1.x;
            vert1.x = (cos(1.5708) * vert1.x + sin(1.5708) * vert1.z) + (i - grid_lenght / 2);
            vert1.z = (-sin(1.5708) * tempX + cos(1.5708) * vert1.z);

            tempX = vert2.x;
            vert2.x = (cos(1.5708) * vert2.x + sin(1.5708) * vert2.z) + (i - grid_lenght / 2);
            vert2.z = -sin(1.5708) * tempX + cos(1.5708) * vert2.z;

            grid.vertices.push_back(vert1);
            grid.vertices.push_back(vert2);

            grid.colors.push_back(color);
            grid.colors.push_back(color);


        }

    }

    setup_vertex_position_buffer_object(grid);
    setup_vertex_color_buffer_object(grid);
    //setup_vertex_normal_buffer_object_tri(grid, true);


}

void MyGLWidget::initGridUnderlayingPane(){
    float grid_lenght = 200;//dimensions.getGridSize();
    glm::vec3 gridPosition = glm::vec3(0, 0, 0); //dimensions.getGridPosition();



    gridUnderlayingPane.vertices.push_back(vec3(gridPosition.x - grid_lenght / 2, gridPosition.y - 0.1, gridPosition.z - grid_lenght / 2));
    gridUnderlayingPane.vertices.push_back(vec3(gridPosition.x - grid_lenght / 2, gridPosition.y - 0.1, gridPosition.z + grid_lenght / 2));
    gridUnderlayingPane.vertices.push_back(vec3(gridPosition.x + grid_lenght / 2, gridPosition.y - 0.1, gridPosition.z + grid_lenght / 2));

    gridUnderlayingPane.vertices.push_back(vec3(gridPosition.x + grid_lenght / 2, gridPosition.y - 0.1, gridPosition.z + grid_lenght / 2));
    gridUnderlayingPane.vertices.push_back(vec3(gridPosition.x + grid_lenght / 2, gridPosition.y - 0.1, gridPosition.z - grid_lenght / 2));
    gridUnderlayingPane.vertices.push_back(vec3(gridPosition.x - grid_lenght / 2, gridPosition.y - 0.1, gridPosition.z - grid_lenght / 2));

    gridUnderlayingPane.colors.push_back(gridPaneColor);
    gridUnderlayingPane.colors.push_back(gridPaneColor);
    gridUnderlayingPane.colors.push_back(gridPaneColor);
    gridUnderlayingPane.colors.push_back(gridPaneColor);
    gridUnderlayingPane.colors.push_back(gridPaneColor);
    gridUnderlayingPane.colors.push_back(gridPaneColor);


    setup_vertex_position_buffer_object(gridUnderlayingPane);
    setup_vertex_color_buffer_object(gridUnderlayingPane);
    setup_vertex_normal_buffer_object_tri(gridUnderlayingPane, true);
}



void MyGLWidget::initMesh(char *model_path) {
    trig.LoadFile(model_path);
    mesh.vertices = trig.Vertices();
    for (glm::vec3 vert : mesh.vertices) {
        mesh.colors.push_back(faceColor);
    }

    setup_vertex_position_buffer_object(mesh);
    setup_vertex_normal_buffer_object_tri(mesh, true);
    setup_vertex_color_buffer_object(mesh);

    meshPoints.vertices = trig.Vertices();
    for (glm::vec3 vert : meshPoints.vertices) {
        meshPoints.colors.push_back(pointsColor);
    }

    setup_vertex_position_buffer_object(meshPoints);
    setup_vertex_normal_buffer_object_tri(meshPoints, true);
    setup_vertex_color_buffer_object(meshPoints);

}


void MyGLWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    camera->Update();
    camera->GetMatricies(projectionMatrix, viewMatrix, modelMatrix);

    mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;    //Compute the mvp matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(value_ptr(mvp));
    glMatrixMode(GL_MODELVIEW);


    shader.Bind();
    // pass uniform variables to shader
    passUniformToShader();


    render(meshPoints, GL_POINTS);
    render(mesh, GL_TRIANGLES);

    if(drawGrid) {
        render(grid, GL_LINES);
        render(gridUnderlayingPane, GL_TRIANGLES);
    }


    shader.Unbind();


}

void MyGLWidget::render(Object &object, int gl_draw_type) {


    //compute model-matrix of object
    GLint modelMatrix_location = glGetUniformLocation(shader.ID(), "modelMatrix");
    glUniformMatrix4fv(modelMatrix_location, 1, GL_FALSE, &object.model[0][0]);


    // bind vertex positions to shader
    GLint position_location = glGetAttribLocation(shader.ID(), "vertex_position");
    if (position_location != -1) {
        glEnableVertexAttribArray(position_location);
        glBindBuffer(GL_ARRAY_BUFFER, object.vertex_position_buffer);
        glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    // bind vertex normals to shader
    GLint normal_location = glGetAttribLocation(shader.ID(), "vertex_normal");
    if (normal_location != -1) {
        glEnableVertexAttribArray(normal_location);
        glBindBuffer(GL_ARRAY_BUFFER, object.vertex_normal_buffer);
        glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    // bind color to shader
    GLint color_location = glGetAttribLocation(shader.ID(), "v_color");
    if (color_location != -1) {
        glEnableVertexAttribArray(color_location);
        glBindBuffer(GL_ARRAY_BUFFER, object.vertex_color_buffer);
        glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    glPointSize(10); //dimensions

    glDrawArrays(gl_draw_type, 0, object.vertices.size());
    glDisableVertexAttribArray(position_location);
    glDisableVertexAttribArray(normal_location);
    glDisableVertexAttribArray(color_location);

}


void MyGLWidget::passUniformToShader() {
    GLint projectionMatrix_location = glGetUniformLocation(shader.ID(), "projectionMatrix");
    GLint viewMatrix_location = glGetUniformLocation(shader.ID(), "viewMatrix");
    GLint modelMatrix_location = glGetUniformLocation(shader.ID(), "modelMatrix");
    GLint normalMatrix_location = glGetUniformLocation(shader.ID(), "normalMatrix");
    GLint materialAmbient_location = glGetUniformLocation(shader.ID(), "materialAmbient");
    GLint materialDiffuse_location = glGetUniformLocation(shader.ID(), "materialDiffuse");
    GLint materialSpecular_location = glGetUniformLocation(shader.ID(), "materialSpecular");
    GLint lightPosition_location = glGetUniformLocation(shader.ID(), "lightPosition");
    GLint lightAmbient_location = glGetUniformLocation(shader.ID(), "lightAmbient");
    GLint lightDiffuse_location = glGetUniformLocation(shader.ID(), "lightDiffuse");
    GLint lightSpecular_location = glGetUniformLocation(shader.ID(), "lightSpecular");
    GLint lightGlobal_location = glGetUniformLocation(shader.ID(), "lightGlobal");
    GLint materialShininess_location = glGetUniformLocation(shader.ID(), "materialShininess");
    GLint constantAttenuation_location = glGetUniformLocation(shader.ID(), "constantAttenuation");
    GLint linearAttenuation_location = glGetUniformLocation(shader.ID(), "linearAttenuation");
    GLint useTexture_location = glGetUniformLocation(shader.ID(), "useTexture");
    glUniformMatrix4fv(projectionMatrix_location, 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniformMatrix4fv(viewMatrix_location, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(modelMatrix_location, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix3fv(normalMatrix_location, 1, GL_FALSE, &normalMatrix[0][0]);
    glUniform3fv(materialAmbient_location, 1, materialAmbient);
    glUniform3fv(materialDiffuse_location, 1, materialDiffuse);
    glUniform3fv(materialSpecular_location, 1, materialSpecular);
    glUniform3fv(lightPosition_location, 1, lightPosition);
    glUniform3fv(lightAmbient_location, 1, lightAmbient);
    glUniform3fv(lightDiffuse_location, 1, lightDiffuse);
    glUniform3fv(lightSpecular_location, 1, lightSpecular);
    glUniform3fv(lightGlobal_location, 1, lightGlobal);
    glUniform1f(materialShininess_location, materialShininess);
    glUniform1f(constantAttenuation_location, constantAttenuation);
    glUniform1f(linearAttenuation_location, linearAttenuation);
    glUniform1i(useTexture_location, 0);
}

void MyGLWidget::resizeGL(int width, int height) {
    int side = qMin(width, height);
    glViewport(0, 0, width, height);
    camera->SetViewport(0, 0, width, height);

}

void MyGLWidget::mousePressEvent(QMouseEvent *event) {
    camera->SetPos(event);

    if(event->modifiers().testFlag(Qt::ControlModifier)){
        //selectVertex(getWorldCoordinates(event->x(), event->x()), 1, true); //dimensions radius
        cout << "x: " << getWorldCoordinates(event->x(), event->x()).x << " y: " << getWorldCoordinates(event->x(), event->x()).y << " z: " << getWorldCoordinates(event->x(), event->x()).z << endl;
    }

}


void MyGLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        //camera->SetPos(event);
        camera->Move2D(event->x(), event->y());
        updateGL();
    }

}

void MyGLWidget::keyPressEvent(QKeyEvent *event) {

    switch (event->key()) {
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
        case Qt::Key_G:
            drawGrid = !drawGrid;
        default:
            break;
    }
    updateGL();


}


void MyGLWidget::setup_vertex_position_buffer_object(Object &object) {
    glGenBuffers(1, &object.vertex_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, object.vertex_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.vertices.size(),
                 &object.vertices.at(0), GL_STATIC_DRAW);
}

void MyGLWidget::setup_vertex_color_buffer_object(Object &object) {
    glGenBuffers(1, &object.vertex_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, object.vertex_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.colors.size(),
                 &object.colors.at(0), GL_STATIC_DRAW);
}

void MyGLWidget::setup_vertex_normal_buffer_object_tri(Object &object, bool smoothed) {

    std::vector<glm::vec3> normals;
    if (smoothed) {
        // initialize map of normals to zero
        // note that we store readily hashable vector<double> types instead of
        // vec3s and convert between the two as required
        // ...avoids some of the pain using <map> without much C++ knowledge
        std::map<std::vector<double>, std::vector<double> > normal_map;
        for (int i = 0; i < object.vertices.size(); i++) {
            std::vector<double> zeros;
            zeros.push_back(0.0);
            zeros.push_back(0.0);
            zeros.push_back(0.0);
            normal_map[to_vector(object.vertices[i])] = zeros;
        }
        for (int i = 0; i < object.vertices.size(); i += 3) {
            // get vertices of the current triangle
            glm::vec3 v1 = object.vertices[i];
            glm::vec3 v2 = object.vertices[i + 1];
            glm::vec3 v3 = object.vertices[i + 2];
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
        for (int i = 0; i < object.vertices.size(); i++) {
            normals.push_back(to_vec3(normal_map[to_vector(object.vertices[i])]));
        }
    } else {
        for (int i = 0; i < object.vertices.size(); i += 3) {
            // get vertices of this triangle
            glm::vec3 v1 = object.vertices[i];
            glm::vec3 v2 = object.vertices[i + 1];
            glm::vec3 v3 = object.vertices[i + 2];
            // compute face normal
            glm::vec3 face_normal = glm::cross(v3 - v2, v1 - v2);
            normals.push_back(glm::normalize(face_normal));
            normals.push_back(glm::normalize(face_normal));
            normals.push_back(glm::normalize(face_normal));
        }
    }
    glGenBuffers(1, &object.vertex_normal_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, object.vertex_normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
                 &normals[0], GL_STATIC_DRAW);
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

    //cout << objx << " , " << objy << " , " << objz << "\n";

    return vec3(objx, objy, objz);

}



