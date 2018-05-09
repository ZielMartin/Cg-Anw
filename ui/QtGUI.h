#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGUI.h"

class QtGUI : public QMainWindow
{
	Q_OBJECT

public:
	QtGUI(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtGUIClass ui;
};
