//
// Created by Johannes on 30.04.2018.
//

#ifndef CG_ANW_OGLWIDGET_H
#define CG_ANW_OGLWIDGET_H

#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <memory>
#include "camera.h"
#include "HalfEdge.h"
#include "OBJDimensions.h"

#define WIDOWWIDTH 1024
#define WIDOWHEIGHT 600

namespace cg {



    class OGLWidgetUI;

    class OGLWidget{
    public:
        OGLWidget(const OBJDimensions &dimensions);

        void initOGLWidget(int argc, char **argv, const std::shared_ptr<cg::HE_Wrapper> wrapper);

        glm::vec3 getWorldCoordinates(int x, int y);

        const std::shared_ptr<HE_Wrapper> getWrapperPtr() const;

        Camera *getCamera() const;

        bool isGrid() const;

        void setGrid(bool grid);

        void display();

        void motionFunc(int x, int y);

        void timerFunc(int value);

        void CloseFunc();

        void ReshapeFunc(int w, int h);

        const OBJDimensions &getDimensions() const;

    private:

        glm::vec3 backgroundColor;
        glm::vec3 gridPaneColor;
        glm::vec3 selectedSphereColor;
        glm::vec3 sphereColor;
        glm::vec3 gridColor;
        glm::vec3 faceColor;


        std::shared_ptr<cg::HE_Wrapper> wrapperPtr;
        cg::Camera *camera;
        bool grid;

        OBJDimensions dimensions;


        void drawSpheres();

        void drawFaces();

        void drawGridPane();

        void drawGrid();

        void drawNormals();

        ;

    };


}






#endif //CG_ANW_OGLWIDGET_H
