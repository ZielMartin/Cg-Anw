// Window.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QSlider>
#include "glwidget.h"

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
    void userControl();
    void meshInfo();
    void sliderValueChanged(int);



private:
    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;


    QMenu *vertexMenu;
    QAction *vertexWeightAct;


    QMenu *infoMenu;
    QAction *userControlAct;
    QAction *meshInfoAct;


    QSlider *slider;



    GLWidget *glWidget;


};

#endif