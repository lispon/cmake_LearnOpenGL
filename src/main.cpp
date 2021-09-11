#include "config.h"
#include "startshader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(int argc, char* argv[]) {
    std::cout << "VERSION:" << VERSION_MAJOR << "." << VERSION_MINOR << std::endl;

    StartShader wgt;
    wgt.Show();

    return 0;
}
