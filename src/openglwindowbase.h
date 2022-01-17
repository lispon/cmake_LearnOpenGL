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
    virtual void ConstructData() = 0;
    //
    virtual void DestructData() = 0;
    //
    virtual void ProcessInput();
    //
    virtual void Draw() = 0;
    //
    void Show();

  protected:
    GLFWwindow* _glfw_window;

  private:
    static int _unique;
    const int _width = 800, _height = 600;
    //
    bool InitWindow();
};

#endif // OPENGLWINDOWBASE_H
