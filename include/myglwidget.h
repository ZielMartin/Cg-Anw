// myglwidget.h

#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H


#include <QGLWidget>
#include "camera.h"


#include "TriangleMesh.h"    // model loading
#include "Shader.h"          // compiling shaders
#include "utils.h"           // generic helper functions
#include "scene_constants.h" // material and light properties



class MyGLWidget : public QGLWidget
{
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
    void draw();

    void setup_vertex_position_buffer_object(void);

    void setup_vertex_uv_buffer_object(void);

    void setup_vertex_normal_buffer_object(bool smoothed);

    glm::mat3 get_default_normalMatrix(void);

    glm::mat4 get_default_viewMatrix(void);

    glm::mat4 get_default_modelMatrix(void);

    glm::mat4 get_default_projectionMatrix(void);



    cg::Camera *camera;

    glm::mat4 projectionMatrix, viewMatrix, modelMatrix;
    glm::mat3 normalMatrix;

    GLuint vertex_position_buffer, vertex_normal_buffer, vertex_uv_buffer;


    TriangleMesh trig;
    Shader shader;


    int xRot;
    int yRot;
    int zRot;

    QPoint lastPos;
};

#endif // MYGLWIDGET_H

