//
// Created by Johannes on 30.04.2018.
//


#include "OGLWidget.h"
#include "OGLWidgetUI.h"



using namespace std;
using namespace glm;

namespace cg {
//Create the Camera
    Camera camera;


    std::shared_ptr<HE_Wrapper> wrapperPtr;


    vec3 backgroundColor = vec3(0, 0, 0);
    vec3 gridPaneColor = vec3(0.2, 0.2, 0.2);
    vec3 selectedSphereColor = vec3(1, 0, 0);
    vec3 sphereColor = vec3(1, 1, 1);
    vec3 gridColor = vec3(1, 1, 1);
    vec3 faceColor = vec3(0.2, 0.4, 0.4);
    bool grid = true;

    const float light0_position[4] = {0,100,0,1};

    GLuint  prog_hdlr;
    GLint location_attribute_0, location_viewport;



    class Window {
    public:
        Window() {
            this->interval = 1000 / 60;        //60 FPS
            this->window_handle = -1;
        }

        int window_handle, interval;
        ivec2 size;
        float window_aspect;
    } window;


    void drawSpheres() {

            for (int i = 0; i < wrapperPtr->getVerts().size(); i++) {
                shared_ptr<HE_vert> vertex =  wrapperPtr->getVerts().at(i);
                glPushMatrix();
                if (wrapperPtr->getVerts().at(i)->selected) {
                    glColor3f(selectedSphereColor.r, selectedSphereColor.g, selectedSphereColor.b);
                } else {
                    glColor3f(sphereColor.r, sphereColor.g, sphereColor.b);

                }

                glTranslatef(vertex->pos.x, vertex->pos.y, vertex->pos.z);
                glutSolidSphere(SPHERERADIUS, SPHERESLICES, SPHERESTACKS);
                glPopMatrix();
            }


    }

/**
 * grid underlying square.
 * necessary because mouse position is only correct detected while clicking on objects
 */
//TODO: make invisible
    void drawGridPane() {
        glPushMatrix();
        glColor3f(gridPaneColor.r, gridPaneColor.g, gridPaneColor.b);
        glBegin(GL_POLYGON);
        glVertex3f(-GRIDLENGTH / 2, -0.1, -GRIDLENGTH / 2);
        glVertex3f(GRIDLENGTH / 2, -0.1, -GRIDLENGTH / 2);
        glVertex3f(GRIDLENGTH / 2, -0.1, GRIDLENGTH / 2);
        glVertex3f(-GRIDLENGTH / 2, -0.1, GRIDLENGTH / 2);
        glEnd();
        glPopMatrix();

    }

    void drawGrid() {
        int i;
        for (i = -GRIDLENGTH; i <= GRIDLENGTH; i++) {
            if (i <= 0) {
                glPushMatrix();
                glTranslatef(0, 0, i + GRIDLENGTH / 2);
                glLineWidth(1);
                glBegin(GL_LINES);
                glColor3f(gridColor.r, gridColor.g, gridColor.b);
                glVertex3f(-GRIDLENGTH / 2, 0, 0);
                glVertex3f(GRIDLENGTH / 2, 0, 0);
                glEnd();
                glPopMatrix();


            }
            if (i >= 0) {
                glPushMatrix();
                glTranslatef(i - GRIDLENGTH / 2, 0, 0);
                glRotatef(-90, 0, 1, 0);
                glLineWidth(1);
                glBegin(GL_LINES);
                glColor3f(gridColor.r, gridColor.g, gridColor.b);
                glVertex3f(-GRIDLENGTH / 2, 0, 0);
                glVertex3f(GRIDLENGTH / 2, 0, 0);
                glEnd();
                glPopMatrix();

            }

        }


    }

    void drawFaces() {

        glPushMatrix();
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(faceColor.r, faceColor.g, faceColor.b);
        //GLfloat cyan[] = {0.f, .8f, .8f, 1.f};
        //glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);

        for (FacePointer fp : wrapperPtr->getFaces()) {
            EdgePointer start = fp->edge;
            EdgePointer curr = start;
            if(start != NULL){
                do{
                    glVertex3f(curr->vert->pos.x, curr->vert->pos.y,
                               curr->vert->pos.z);
                    curr = curr->next;
                }while(curr != start);
            }

        }
        glEnd();
        glPopMatrix();
    }

    void display() {

        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, window.size.x, window.size.y);

        mat4 model, view, projection;
        camera.Update();
        camera.GetMatricies(projection, view, model);

        mat4 mvp = projection * view * model;    //Compute the mvp matrix
        glLoadMatrixf(value_ptr(mvp));



        if (grid) {
            glDisable(GL_LIGHT0);
            drawGrid();
            drawGridPane();
            glEnable(GL_LIGHT0);

        }
        drawSpheres();
        drawFaces();
        glutSwapBuffers();
    }

//Used when person drags mouse around
    void CallBackMotionFunc(int x, int y) {
        camera.Move2D(x, y);
    }

//Redraw based on fps set for window
    void TimerFunc(int value) {
        if (window.window_handle != -1) {
            glutTimerFunc(window.interval, TimerFunc, value);
            glutPostRedisplay();
        }
    }


//Invalidate the window handle when window is closed
    void CloseFunc() {
        window.window_handle = -1;
    }

//Resize the window and properly update the camera viewport
    void ReshapeFunc(int w, int h) {
        if (h > 0) {
            window.size = ivec2(w, h);
            window.window_aspect = float(w) / float(h);
        }
        camera.SetViewport(0, 0, window.size.x, window.size.y);
    }

    void printInfoLog(GLuint obj) {
        int log_size = 0;
        int bytes_written = 0;
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &log_size);
        if (!log_size) return;
        char *infoLog = new char[log_size];
        glGetProgramInfoLog(obj, log_size, &bytes_written, infoLog);
        std::cerr << infoLog << std::endl;
        delete [] infoLog;
    }


    bool read_n_compile_shader(const char *filename, GLuint &hdlr, GLenum shaderType) {
        std::ifstream is(filename, std::ios::in|std::ios::binary|std::ios::ate);
        if (!is.is_open()) {
            std::cerr << "Unable to open file " << filename << std::endl;
            return false;
        }
        long size = is.tellg();
        char *buffer = new char[size+1];
        is.seekg(0, std::ios::beg);
        is.read (buffer, size);
        is.close();
        buffer[size] = 0;

        hdlr = glCreateShader(shaderType);
        glShaderSource(hdlr, 1, (const GLchar**)&buffer, NULL);
        glCompileShader(hdlr);
        std::cerr << "info log for " << filename << std::endl;
        printInfoLog(hdlr);
        delete [] buffer;
        return true;
    }

    void setShaders(GLuint &prog_hdlr, const char *vsfile, const char *fsfile, const char *gsfile) {
        GLuint vert_hdlr, frag_hdlr, geom_hdlr;
        read_n_compile_shader(vsfile, vert_hdlr, GL_VERTEX_SHADER);
        read_n_compile_shader(gsfile, geom_hdlr, GL_GEOMETRY_SHADER);
        read_n_compile_shader(fsfile, frag_hdlr, GL_FRAGMENT_SHADER);


        prog_hdlr = glCreateProgram();
        glAttachShader(prog_hdlr, vert_hdlr);
        glAttachShader(prog_hdlr, geom_hdlr);
        glAttachShader(prog_hdlr, frag_hdlr);



        glLinkProgram(prog_hdlr);
        std::cerr << "info log for the linked program" << std::endl;
        printInfoLog(prog_hdlr);
    }


    void initOGLWidget(int argc, char **argv, std::shared_ptr<HE_Wrapper> wrapper) {

        wrapperPtr = wrapper;

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
        glutInitWindowSize(WIDOWWIDTH, WIDOWHEIGHT);

        //Setup window and callbacks
        window.window_handle = glutCreateWindow("modeller");
        glutReshapeFunc(ReshapeFunc);
        glutDisplayFunc(display);
        glutKeyboardFunc(keyboard);
        glutSpecialFunc(specialKeyboard);
        glutMouseFunc(mouseClicks);
        glutMotionFunc(CallBackMotionFunc);
        glutTimerFunc(window.interval, TimerFunc, 0);

        glewExperimental = GL_TRUE;

        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        //glEnable(GL_DIFFUSE);
        //glEnable(GL_AMBIENT);
        //glEnable(GL_SPECULAR);

        glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

        if (glewInit() != GLEW_OK) {
            cerr << "GLEW failed to initialize." << endl;
        }
        if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GL_EXT_geometry_shader4)
            std::cout << "Ready for GLSL - vertex, fragment, and geometry units" << std::endl;
        else {
            std::cout << "No GLSL support" << std::endl;
            exit(1);
        }

        setShaders(prog_hdlr, "shader/vert_shader.glsl", "shader/frag_shader.glsl", "shader/geom_shader.glsl");

        location_attribute_0   = glGetAttribLocation(prog_hdlr, "R");          // radius
        location_viewport = glGetUniformLocation(prog_hdlr, "viewport"); // viewport

        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        //Setup camera
        camera.SetMode(FREE);
        camera.SetPosition(vec3(20, 10, 10));
        camera.SetLookAt(vec3(0, 0, 0));
        camera.SetClipping(.1, 1000);
        camera.SetFOV(45);
        //Start the glut loop!
        glutMainLoop();

    }


    vec3 getWorldCoordinates(int x, int y) {
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


}











