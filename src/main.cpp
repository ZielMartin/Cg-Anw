#include "OGLWidget.h"

int main(int argc, char** argv) {
    cg::HE_Wrapper *wrapper = new cg::HE_Wrapper();
    std::shared_ptr<cg::HE_Wrapper> wrapperPtr(wrapper);
    initOGLWidget(argc, argv, wrapperPtr);
    std::cout << "TEST2";
    return 0;
}











