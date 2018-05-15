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


#define WIDOWWIDTH 1024
#define WIDOWHEIGHT 600

namespace cg {



    class OGLWidgetUI;

    class OGLWidget{
    public:
        OGLWidget();

        void initOGLWidget(int argc, char **argv, const std::shared_ptr<cg::HE_Wrapper> wrapper);

        glm::vec3 getWorldCoordinates(int x, int y);




        float getSphere_radius() const;

        void setSphere_radius(float sphere_radius);

        float getSphere_slices() const;

        void setSphere_slices(float sphere_slices);

        float getSphere_stacks() const;

        void setSphere_stacks(float sphere_stacks);

        int getGrid_lenght() const;

        void setGrid_lenght(int grid_lenght);

        const std::shared_ptr<HE_Wrapper> getWrapperPtr() const;

        void setWrapperPtr(const std::shared_ptr<HE_Wrapper> wrapperPtr);

        Camera *getCamera() const;


        bool isGrid() const;

        void setGrid(bool grid);

        float getMove_step_Size() const;

        void setMove_step_Size(float move_step_Size);

        void display();

        void motionFunc(int x, int y);

        void timerFunc(int value);

        void CloseFunc();

        void ReshapeFunc(int w, int h);

    private:
        float sphere_radius;
        float sphere_slices;
        float sphere_stacks;
        int grid_lenght;
        float move_step_Size;


        std::shared_ptr<cg::HE_Wrapper> wrapperPtr;
        cg::Camera *camera;
        bool grid;





        void drawSpheres();

        void drawFaces();

        void drawGridPane();

        void drawGrid();

        void drawNormals();

        ;

    };


}






#endif //CG_ANW_OGLWIDGET_H
