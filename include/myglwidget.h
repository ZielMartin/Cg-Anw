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

#include "VBOHandler.h"
#include <QGLWidget>
#include "camera.h"

#include "TriangleMesh.h"
#include "Shader.h"
#include "utils.h"
#include "scene_constants.h"


class MyGLWidget : public QGLWidget {
Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);

    ~MyGLWidget() override;

signals:

public slots:

protected:
    void initializeGL() override;

    void paintGL() override;

    void resizeGL(int width, int height) override;

    QSize minimumSizeHint() const override;

    QSize sizeHint() const override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;


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


    glm::vec3 getWorldCoordinates(int x, int y);


    VBOHandler vboHandler;
    cg::Camera *camera;

    glm::mat4 projectionMatrix, viewMatrix, modelMatrix;
    glm::mat3 normalMatrix;


    Shader shader;

    glm::vec3 backgroundColor;



    int xRot;
    int yRot;
    int zRot;

    QPoint lastPos;

};

#endif // MYGLWIDGET_H

