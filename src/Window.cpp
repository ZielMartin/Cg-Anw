// Window.cpp

#include <QtWidgets>

#include "Window.h"




Window::Window() {
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);

    glWidget = new MyGLWidget(this);
    glWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(topFiller);
    layout->addWidget(glWidget);
    widget->setLayout(layout);

    createActions();
    createMenus();


    setWindowTitle(tr("Menus"));
    setMinimumSize(160, 160);
    resize(480, 320);
}


void Window::newFile() {
    glWidget->openMesh(nullptr);
}

void Window::open() {

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open OBJ File",
                                                    QDir::currentPath(),
                                                    "OBJ Files (*.obj)");
    if (!fileName.isEmpty()){
        char* cstr;
        std::string fname = fileName.toStdString();
        cstr = new char [fname.size()+1];
        strcpy( cstr, fname.c_str() );

        glWidget->openMesh(cstr);
        }
}

void Window::save() {
    QString filename = QFileDialog::getSaveFileName(this, "Save OBJ File",  QDir::currentPath(), "*.obj");
    QFile f( filename );

    char* cstr;
    std::string fname = filename.toStdString();
    cstr = new char [fname.size()+1];
    strcpy( cstr, fname.c_str() );

    glWidget->saveOBJ(cstr);
}


void Window::createActions() {
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &Window::newFile);

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &Window::open);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the file to disk"));
    connect(saveAct, &QAction::triggered, this, &Window::save);


    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);




}

void Window::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);



    helpMenu = menuBar()->addMenu(tr("&Help"));


}