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
 */

#include "OGLWidgetUI.h"
#include "OGLWidget.h"

namespace cg {

    void keyboard(unsigned char key, int x, int y) {


        //std::cout << "keyboard: " << key << std::endl;

        VertList vl = wrapperPtr->getFaceVerts();

        switch (key) {
            case 'i':
                for(VertPointer p : wrapperPtr->getFaceVerts()){
                    std::cout << "x:" << p->pos.x << " y:" << p->pos.y << " z:" << p->pos.z << std::endl;
                }
                break;
            case 'w':
                camera.Move(FORWARD);
                break;
            case 's':
                camera.Move(BACK);
                break;
            case 'e':
                camera.Move(UP);
                break;
            case 'a':
                camera.Move(LEFT);
                break;
            case 'q':
                camera.Move(DOWN);
                break;
            case 'd':
                camera.Move(RIGHT);
                break;
            case '-':            //DOWN
                wrapperPtr->moveSelectedVertices(glm::vec3(0.0, -MOVESTEPSIZE, 0.0));
                break;
            case '+':            //UP
                wrapperPtr->moveSelectedVertices(glm::vec3(0.0, MOVESTEPSIZE, 0.0));
                break;
            case 'r':
                wrapperPtr->deleteSelectedVertices();
                break;
            case 'g':
                grid = !grid;
                break;
            case 'f':
                wrapperPtr->createFace(vl);
                break;
            default:
                break;
        }

        glutPostRedisplay();
    }

    void specialKeyboard(int key, int x, int y) {
        //std::cout << "specialKeyboard: " << key << std::endl;
        switch (key) {
            case 101:              //UP ARROW
                wrapperPtr->moveSelectedVertices(glm::vec3(-MOVESTEPSIZE, 0.0, 0.0));
                break;
            case 100:            //LEFT ARROW
                wrapperPtr->moveSelectedVertices(glm::vec3(0.0, 0.0, MOVESTEPSIZE));
                break;
            case 103:            //DOWN ARROW
                wrapperPtr->moveSelectedVertices(glm::vec3(MOVESTEPSIZE, 0.0, 0.0));
                break;
            case 102:            //RIGHT ARROW
                wrapperPtr->moveSelectedVertices(glm::vec3(0.0, 0.0, -MOVESTEPSIZE));
                break;
            default:
                break;
        }

        glutPostRedisplay();
    }

    void mouseClicks(int button, int state, int x, int y) {
        camera.SetPos(button, state, x, y);
        glm::vec3 worldCoordinates = getWorldCoordinates(x, y);

        static std::shared_ptr<glm::vec4> vertA;
        static std::shared_ptr<glm::vec4> vertB;

        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN &&
            glutGetModifiers() == GLUT_ACTIVE_CTRL) {     //selecting mode


            wrapperPtr->selectVertex(worldCoordinates, SPHERERADIUS, true);

        } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN &&
                   glutGetModifiers() != GLUT_ACTIVE_ALT) {    //add Vertex mode

            wrapperPtr->resetSelected();


            VertPointer vp = wrapperPtr->createVert(glm::vec4(worldCoordinates,1));
            wrapperPtr->addFaceVert(vp);


        } else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN &&
                   glutGetModifiers() == GLUT_ACTIVE_ALT) {     //add Edge mode

            //first define vertA
            if (vertA == nullptr) {
                //vertA = vw->selectVertex(worldCoordinates, SPHERERADIUS, false);

            } else if (vertB == nullptr) {  //then vertB
                //vertB = vw->selectVertex(worldCoordinates, SPHERERADIUS, false);

                if (vertB.get() != vertA.get() && vertB != nullptr) {
                    //OGLWEdgesWrapper *vecW = OGLWEdgesWrapper::getInstance();
                    //vecW->addEdge(vertA, vertB);
                } else {
                    //if double click on vertex or no vertex clicked
                    vertB = nullptr;
                    vertA = nullptr;
                }
            } else {
                //vertA = vw->selectVertex(worldCoordinates, SPHERERADIUS, false);
                //vertB = nullptr;
            }


        }
    }

}