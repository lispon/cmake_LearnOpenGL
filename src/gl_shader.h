#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class GLShader {

public:
    GLShader(std::string vertex_path, std::string fragment_path);
    void Use();
    unsigned int GetID() const;

private:
    unsigned int _id;
    void CheckCompileErrors(unsigned int shader, std::string type);
};

#endif // GL_SHADER_H
