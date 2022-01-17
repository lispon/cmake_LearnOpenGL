#include "config.h"
#include "startshader.h"
#include "starttextures.h"

#include "hello_window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(int argc, char* argv[]) {
    std::cout << "VERSION:" << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;
    if(1 == argc) {
        std::cout << "using default empty window." << std::endl;
        std::cerr << "don't support any arguments:" << argv[1] << " ..." << std::endl;
        return HelloWindow();
    }

//    StartShader wgt1;
//    wgt1.Show();

    StartTextures wgt;
    wgt.Show();

    return 0;
}
