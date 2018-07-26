// Window.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QSlider>

#include <QPointer>
#include <QGridLayout>
#include <QCheckBox>
//#include "ui_window.h"
#include "glwidget.h"

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

namespace Ui {
    class Window;
}

class Window : public QMainWindow
{
Q_OBJECT

public:
    explicit Window(QWidget *parent = Q_NULLPTR);

public slots:
    void sharp(bool sharp);
    void setCheckBox(bool sharp);

private slots:
    void newFile();
    void open();
    void save();
    void vertexWeight();
    void userControl();
    void meshInfo();
    void sliderValueChanged(float);
    void limitCecked();



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
    QPointer<QAction> limitCeckedBtn;
    QPointer<QAction> sharpEdgeChecked;

    QSlider *slider;
    QCheckBox *checkbox;


    GLWidget *glWidget;
    //Ui::Window ui;

};

#endif
