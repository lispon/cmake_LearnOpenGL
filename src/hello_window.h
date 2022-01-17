#ifndef HELLO_WINDOW_H
#define HELLO_WINDOW_H

#include <cassert>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
    if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
}

int HelloWindow() {
    {
        // init glfw.
        glfwInit();
        // https://www.glfw.org/docs/latest/window.html#window_hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  # on Mac OS X.
#endif
    }

    GLFWwindow* window = nullptr;
    {
        const unsigned int wdw_width = 800;
        const unsigned int wdw_height = 600;
        window = glfwCreateWindow(wdw_width, wdw_height, "title", nullptr, nullptr);
        if(window) {
            std::cout << "Successed to create GLFW window." << std::endl;
        } else {
            std::cout << "Failed to create GLFW window." << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    }
    assert(window);

    {
        if(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::cout << "Successed to initialize GLAD." << std::endl;
        } else {
            std::cout << "Failed to initialize GLAD." << std::endl;
            glfwTerminate();
            return -1;
        }
    }


    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

#endif // HELLO_WINDOW_H
