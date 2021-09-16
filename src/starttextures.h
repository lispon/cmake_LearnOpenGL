#ifndef STARTTEXTURES_H
#define STARTTEXTURES_H

#include "openglwindowbase.h"

class StartTextures : public OpenGLWindowBase {
  public:
    StartTextures();
    ~StartTextures();

    void ConstructData() override;
    void DestructData() override;
    void Draw() override;
    void ProcessInput() override;

  private:
    void SetTextureOpacity(bool up);


  private:
    Shader* _shader;

    float _opacity;

    unsigned int _vao;
    unsigned int _vbo;
    unsigned int _ebo;
    unsigned int _texture;
    unsigned int _texture2;
};

#endif // STARTTEXTURES_H
