// Window.cpp

#include <QtWidgets>

#include "Window.h"




Window::Window() {
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);

    glWidget = new GLWidget(this);
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

void Window::vertexWeight() {

    bool ok;
    double input = QInputDialog::getDouble(this, tr("VertexWeight"),
                                          tr("Vertex weight:"), 0, -2147483647, 2147483647, 1, &ok);
    if (ok){
        glWidget->setVertexWeight((float) input);
    }


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

void Window::userControl() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("User Interaction");
    msgBox.setText("Move camera direction:\tpress left mouse-button and move mouse\n"
                   "Move camera position:\tuse keys: 'w','a','s','d','q','e'\n\n"
                   "Disable/Enable grid:\t\tpress 'g'\n"
                   "Disable/Enable points:\t\tpress 'p'\n\n"
                   "Select point:\t\t\tCtrl + left-click on point\n"
                   "Add new point:\t\tright-click on grid or object\n"
                   "Delete selected points:\t\tpress 'r'\n"
                   "Move selected points:\t\tarrow-keys,'+','-'\n\n"
                   "Create Face:\t\t\tselect vertices in counterclockwise order and press 'f'\n\n"
                   "CatmullClark subdivision:\tpress 'c' for one Step\n"
                   "Undo subdivision step:\tCtrl + 'z'\n"
                    );

    msgBox.exec();
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

void Window::meshInfo(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("Mesh Info");
    QString text;
    std::vector<std::pair<std::string, int>> stats = glWidget->meshInfo();
    for(std::pair<std::string, int> p : stats){
        text.append(QString::fromStdString(p.first));
        text.append(":");
        text.append("\t");
        text.append(QString::number(p.second));
        text.append("\n");
    }
    msgBox.setText(text);

    msgBox.exec();

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

    vertexWeightAct = new QAction(tr("&VertexWeight"), this);
    vertexWeightAct->setStatusTip(tr("Change weight of selected vertices for subdivision"));
    connect(vertexWeightAct, &QAction::triggered, this, &Window::vertexWeight);

    userControlAct = new QAction(tr("&Interaction"), this);
    userControlAct->setStatusTip(tr("Help for user-interaction"));
    connect(userControlAct, &QAction::triggered, this, &Window::userControl);

    meshInfoAct = new QAction(tr("&Mesh Info"), this);
    meshInfoAct->setStatusTip(tr("Information about the mesh"));
    connect(meshInfoAct, &QAction::triggered, this, &Window::meshInfo);





}

void Window::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    vertexMenu = menuBar()->addMenu(tr("&Vertex"));
    vertexMenu->addAction(vertexWeightAct);



    infoMenu = menuBar()->addMenu(tr("&Info"));
    infoMenu->addAction(userControlAct);
    infoMenu->addAction(meshInfoAct);



}