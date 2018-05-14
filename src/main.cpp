#include "OGLWidget.h"
#include "OBJLoader.h"

int main(int argc, char** argv) {


    cg::OBJLoader *objLoader = new cg::OBJLoader();
    objLoader->readFile("../obj/suzanne.obj");
    std::shared_ptr<cg::HE_Wrapper> wrapperPtr(&objLoader->getHalfEdgeStruct());

    //cg::HE_Wrapper *wrapper = new cg::HE_Wrapper();
    //std::shared_ptr<cg::HE_Wrapper> wrapperPtr(wrapper);

    initOGLWidget(argc, argv, wrapperPtr);
    return 0;
}











