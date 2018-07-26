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

#include "Renderer.h"
#include <QGLWidget>
#include <QVector4D>
#include "camera.h"

#include "Shader.h"
#include "utils.h"
#include "scene_constants.h"


class GLWidget : public QGLWidget {
Q_OBJECT

signals:
    void valueChanged(bool sharp);

public:
    explicit GLWidget(QWidget *parent = 0, QGLFormat format = QGLFormat::defaultFormat());

    ~GLWidget() override;

    void openMesh(char *model_path);

    void saveOBJ(char *file);

    void setVertexWeight(float weight);
    void setLimitRules(bool LR);
    std::vector<std::pair<std::string, int>> meshInfo();

    void applySmoothedVertices(int interpolationValue);
    void intersect(const QVector3D& origin, const QVector3D& direction) ;
    void setSharpEdge(bool sharp);
    Renderer renderer;
    HE_MESH::HalfedgeHandle selectedEdge;
protected:
    void initializeGL() override;

    void paintGL() override;

    void resizeGL(int width, int height) override;

    QSize minimumSizeHint() const override;

    QSize sizeHint() const override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;



private:


    glm::vec3 getWorldCoordinates(int x, int y);




    cg::Camera *camera;

    glm::mat4 projectionMatrix, viewMatrix, modelMatrix;
    glm::mat3 normalMatrix;
    QVector4D viewport;

    Shader shader;

    glm::vec3 backgroundColor;


};

#endif // MYGLWIDGET_H

