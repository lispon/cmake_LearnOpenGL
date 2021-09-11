#ifndef OPENGLWINDOWBASE_H
#define OPENGLWINDOWBASE_H

#include "shader.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

class OpenGLWindowBase {
  public:
    OpenGLWindowBase();
    ~OpenGLWindowBase();

    //
    virtual void ConstructData();
    //
    virtual void DestructData();
    //
    virtual void ProcessInput();
    //
    virtual void Draw();
    //
    void Show();

  private:
    static int _unique;
    GLFWwindow* _glfw_window;
    const int _width = 800, _height = 600;
    //
    bool InitWindow();
};

#endif // OPENGLWINDOWBASE_H
