// Window.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myglwidget.h"

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

class Window : public QMainWindow
{
Q_OBJECT

public:
    Window();


private slots:
    void newFile();
    void open();
    void save();
    void vertexWeight();



private:
    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QMenu *vertexMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;

    QAction *vertexWeightAct;


    MyGLWidget *glWidget;


};

#endif