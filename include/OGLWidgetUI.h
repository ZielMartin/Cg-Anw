//
// Created by Johannes on 30.04.2018.
//

#ifndef CG_ANW_OGLWIDGETUI_H
#define CG_ANW_OGLWIDGETUI_H

#include "HalfEdge.h"
#include "camera.h"


namespace cg {

    class OGLWidget;

    class OGLWidgetUI{
    public:
        OGLWidgetUI(OGLWidget *caller);

        void keyboard(unsigned char key, int x, int y);

        void mouseClicks(int button, int state, int x, int y);

        void specialKeyboard(int key, int x, int y);

    private:
        OGLWidget *caller;



    };




}
#endif //CG_ANW_OGLWIDGETUI_H
