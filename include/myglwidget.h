// myglwidget.h
//
// Created by Johannes on 30.04.2018.
//

/**
 * User-Interaction-Interface
 * - add point with right-click on grid
 * - select multiple points with ctrl + left-click on point
 * - move point/points arrow-keys and '+','-'
 * - remove selected point/points with 'r'
 *
 * - move camera with with 'w','a','s','d','q','e'
 * - rotate camera with mouse click and drag window
 *
 * - connect a face: mark each point with ctrl + left-click, then press 'f'
 *
 * - enable/disable grid with *g*
 */

#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H


#include <QGLWidget>
#include "camera.h"


#include "TriangleMesh.h"    // model loading
#include "Shader.h"          // compiling shaders
#include "utils.h"           // generic helper functions
#include "scene_constants.h" // material and light properties

struct Object {
    /* IDs for several buffers. */

    GLuint vertex_position_buffer;
    GLuint vertex_normal_buffer;
    GLuint vertex_color_buffer;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;

    /* Model matrix */
    glm::mat4x4 model;
};


class MyGLWidget : public QGLWidget {
Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);

    ~MyGLWidget();

signals:

public slots:

protected:
    void initializeGL();

    void paintGL();

    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;

    QSize sizeHint() const;

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);


public slots:

    // slots for xyz-rotation slider
    void setXRotation(int angle);

    void setYRotation(int angle);

    void setZRotation(int angle);

signals:

    // signaling rotation from mouse movement
    void xRotationChanged(int angle);

    void yRotationChanged(int angle);

    void zRotationChanged(int angle);

private:

    void setup_vertex_position_buffer_object(Object &object);

    void setup_vertex_normal_buffer_object_tri(Object &object, bool smoothed);

    void setup_vertex_color_buffer_object(Object &object);

    void passUniformToShader();

    void render(Object &object, int gl_draw_type);

    void initGrid();

    void initGridUnderlayingPane();

    void initMesh(char *model_path);

    glm::vec3 getWorldCoordinates(int x, int y);


    bool drawGrid;
    cg::Camera *camera;

    glm::mat4 projectionMatrix, viewMatrix, modelMatrix;
    glm::mat3 normalMatrix;

    Object mesh, grid, meshPoints, gridUnderlayingPane;
    std::vector<Object> spheres;



    TriangleMesh trig;
    Shader shader;


    glm::vec3 backgroundColor;
    glm::vec3 gridPaneColor;
    glm::vec3 selectedPointsColor;
    glm::vec3 pointsColor;
    glm::vec3 gridColor;
    glm::vec3 faceColor;


    int xRot;
    int yRot;
    int zRot;

    QPoint lastPos;

};

#endif // MYGLWIDGET_H

