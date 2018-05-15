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

#include "OGLWidget.h"
#include "OGLWidgetUI.h"

namespace cg {


    OGLWidgetUI::OGLWidgetUI(OGLWidget *caller){
        this->caller = caller;
    }



    void OGLWidgetUI::keyboard(unsigned char key, int x, int y) {


        //std::cout << "keyboard: " << key << std::endl;

        VertList vl = caller->getWrapperPtr()->getFaceVerts();

        switch (key) {
            case 'i':
                for(VertPointer p : caller->getWrapperPtr()->getFaceVerts()){
                    std::cout << "x:" << p->pos.x << " y:" << p->pos.y << " z:" << p->pos.z << std::endl;
                }
                break;
            case 'w':
                caller->getCamera()->Move(FORWARD);
                break;
            case 's':
                caller->getCamera()->Move(BACK);
                break;
            case 'e':
                caller->getCamera()->Move(UP);
                break;
            case 'a':
                caller->getCamera()->Move(LEFT);
                break;
            case 'q':
                caller->getCamera()->Move(DOWN);
                break;
            case 'd':
                caller->getCamera()->Move(RIGHT);
                break;
            case '-':            //DOWN
                caller->getWrapperPtr()->moveSelectedVertices(glm::vec3(0.0, -caller->getMove_step_Size(), 0.0));
                break;
            case '+':            //UP
                caller->getWrapperPtr()->moveSelectedVertices(glm::vec3(0.0, caller->getMove_step_Size(), 0.0));
                break;
            case 'r':
                caller->getWrapperPtr()->deleteSelectedVertices();
                break;
            case 'g':
                caller->setGrid(!caller->isGrid());
                break;
            case 'f':
                caller->getWrapperPtr()->createFace(vl);
                break;
            default:
                break;
        }

        glutPostRedisplay();
    }

    void OGLWidgetUI::specialKeyboard(int key, int x, int y) {
        //std::cout << "specialKeyboard: " << key << std::endl;
        switch (key) {
            case 101:              //UP ARROW
                caller->getWrapperPtr()->moveSelectedVertices(glm::vec3(-caller->getMove_step_Size(), 0.0, 0.0));
                break;
            case 100:            //LEFT ARROW
                caller->getWrapperPtr()->moveSelectedVertices(glm::vec3(0.0, 0.0, caller->getMove_step_Size()));
                break;
            case 103:            //DOWN ARROW
                caller->getWrapperPtr()->moveSelectedVertices(glm::vec3(caller->getMove_step_Size(), 0.0, 0.0));
                break;
            case 102:            //RIGHT ARROW
                caller->getWrapperPtr()->moveSelectedVertices(glm::vec3(0.0, 0.0, -caller->getMove_step_Size()));
                break;
            default:
                break;
        }

        glutPostRedisplay();
    }

    void OGLWidgetUI::mouseClicks(int button, int state, int x, int y) {
        caller->getCamera()->SetPos(button, state, x, y);
        glm::vec3 worldCoordinates = caller->getWorldCoordinates(x, y);

        static std::shared_ptr<glm::vec4> vertA;
        static std::shared_ptr<glm::vec4> vertB;

        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN &&
            glutGetModifiers() == GLUT_ACTIVE_CTRL) {     //selecting mode


            caller->getWrapperPtr()->selectVertex(worldCoordinates, caller->getSphere_radius(), true);

        } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN &&
                   glutGetModifiers() != GLUT_ACTIVE_ALT) {    //add Vertex mode

            caller->getWrapperPtr()->resetSelected();


            VertPointer vp = caller->getWrapperPtr()->createVert(glm::vec4(worldCoordinates,1));
            caller->getWrapperPtr()->addFaceVert(vp);


        } else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN &&
                   glutGetModifiers() == GLUT_ACTIVE_ALT) {     //add Edge mode

            //vertIndex define vertA
            if (vertA == nullptr) {
                //vertA = vw->selectVertex(worldCoordinates, sphere_radius, false);

            } else if (vertB == nullptr) {  //then vertB
                //vertB = vw->selectVertex(worldCoordinates, sphere_radius, false);

                if (vertB.get() != vertA.get() && vertB != nullptr) {
                    //OGLWEdgesWrapper *vecW = OGLWEdgesWrapper::getInstance();
                    //vecW->addEdge(vertA, vertB);
                } else {
                    //if double click on vertex or no vertex clicked
                    vertB = nullptr;
                    vertA = nullptr;
                }
            } else {
                //vertA = vw->selectVertex(worldCoordinates, sphere_radius, false);
                //vertB = nullptr;
            }


        }
    }

}