// main.cpp

#include <QApplication>
#include <QDesktopWidget>
#include <QGLFormat>

#include "Window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QGLFormat glFormat;
    glFormat.setVersion( 3, 3 );
    glFormat.setProfile( QGLFormat::CoreProfile );
    glFormat.setSampleBuffers( true );
    QGLFormat::setDefaultFormat(glFormat);

    Window window;
    window.resize(window.sizeHint());
    int desktopArea = QApplication::desktop()->width() *
                     QApplication::desktop()->height();
    int widgetArea = window.width() * window.height();

    window.setWindowTitle("modeller");

    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        window.show();
    else
        window.showMaximized();




    return app.exec();
}
