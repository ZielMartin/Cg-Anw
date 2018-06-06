// window.cpp


#include <QtWidgets>
#include "window.h"
#include "../ui/ui_window.h"
#include "myglwidget.h"


Window::Window(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Window)
{
    ui->setupUi(this);

    connect(ui->myGLWidget, SIGNAL(xRotationChanged(int)), ui->rotXSlider, SLOT(setValue(int)));
    connect(ui->myGLWidget, SIGNAL(yRotationChanged(int)), ui->rotYSlider, SLOT(setValue(int)));
    connect(ui->myGLWidget, SIGNAL(zRotationChanged(int)), ui->rotZSlider, SLOT(setValue(int)));
}

Window::~Window()
{
    delete ui;
}
