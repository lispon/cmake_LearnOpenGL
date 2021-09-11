#ifndef STARTSHADER_H
#define STARTSHADER_H

#include "openglwindowbase.h"
#include <math.h>
#include <memory>

///
/// \brief 学习入门章节, 着色器及其之前的部分. 由于纹理等后续内容对于顶点数据有要求;
/// 所以, 创建单独的类来学习示例教程. 仅在main函数中调用.
///
class StartShader : public OpenGLWindowBase {
  public:
    StartShader();
    ~StartShader();

    //
    void ConstructData() override;
    //
    void DestructData() override;
    //
    void ProcessInput() override;
    //
    void Draw() override;

  private:
    GLFWwindow* _glfw_window;

    /// 顶点数组对象(Vertex Array Object, VAO).
    unsigned int vao{0};
    unsigned int vao1{0};
    /// 顶点缓冲对象(Vertex Buffer Objects, VBO).
    unsigned int vbo{0};
    unsigned int vbo1{0};
    /// 索引缓冲对象(Element Buffer Objects, ebo; 也称Index Buffer Objects, ibo).
    unsigned int ebo{0};

    //
    unsigned int _shader_program1;
    unsigned int _shader_program2;
};

#endif // STARTSHADER_H
