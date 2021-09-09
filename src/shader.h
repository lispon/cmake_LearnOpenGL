#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
  public:
    explicit Shader(const GLchar* vertex_path, const GLchar* fragment_path);
    //
    unsigned int GetID() const;
    //
    void Use();


  private:
    enum ShaderType { UNKNOWN, VERTEX, FRAGMENT };
    unsigned int _id;
    //
    std::string ReadFile(const char* path);
    //
    unsigned int CreateShader(const char* str, ShaderType type);
};

#endif // SHADER_H
