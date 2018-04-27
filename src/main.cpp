#include <stdlib.h>
#include <gl/glut.h>
#include <c++/4.8.3/iostream>


class Point
{
    int Xvalue, Yvalue;
public:
    void xy(int x, int y)
    {
        Xvalue = x;
        Yvalue = y;
    }

    //return individual x y value
    int x() { return Xvalue; }
    int y() { return Yvalue; }
};

Point point[30];
int count = 0;

void Display()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT); // clear display window

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const double w = glutGet( GLUT_WINDOW_WIDTH );
    const double h = glutGet( GLUT_WINDOW_HEIGHT );
    gluOrtho2D(0.0, w, 0.0, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0, 1.0, 1.0);

    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < count; i++)
    {
        int x = point[i].x();
        int y = point[i].y();

        glColor3f(0.3, 0.3, 0.3);
        glVertex2i(x, h - y);
    }
    glEnd();

    glFlush();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        point[count].xy(x, y);
        count++;
    }
    glutPostRedisplay();
}

int main( int argc, char *argv[] )
{
    std::cout<<"asd"<<std::endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(1200, 800);
    glutCreateWindow("NURBS Curve");

    glutMouseFunc(mouse);
    glutDisplayFunc(Display);

    glutMainLoop();
    return 0;
}