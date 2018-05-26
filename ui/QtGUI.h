#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGUI.h"

class QtGUI : public QMainWindow
{
	Q_OBJECT

public:
	QtGUI(QWidget *parent = Q_NULLPTR);

//list all your custom SLOTS here
public slots:
        void mySlot();

private:
	Ui::QtGUIClass ui;
};
