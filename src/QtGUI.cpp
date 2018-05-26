#include "ui/QtGUI.h"
#include <QMessageBox>

QtGUI::QtGUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
        /*Access Component; on specific Signal; Receiver use SLOT(function)
         *alt. you can define this in QT Creator
        */
        connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(close()));
        connect(ui->actionVertexMode, SIGNAL(triggered(bool)), this, SLOT(mySlot()));


    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }

    /*
     * Easy Example for your own Slot
    */
    void MainWindow::mySlot()
    {
        QMessageBox msgBox;
        msgBox.setText("TAADAA");
        msgBox.exec();

    }
