#include "shader.h"

Shader::Shader(const GLchar* vertex_path, const GLchar* fragment_path)
    : _id(0) {
    std::string vertex_src = ReadFile(vertex_path);
    std::string fragment_src = ReadFile(fragment_path);
    auto vertex = CreateShader(vertex_src.c_str(), VERTEX);
    auto fragment = CreateShader(fragment_src.c_str(), FRAGMENT);

    _id = glCreateProgram();
    glAttachShader(_id, vertex);
    glAttachShader(_id, fragment);
    glLinkProgram(_id);
    int success{0};
    char info_log[512] {};
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_id, sizeof(info_log), nullptr, info_log);
        std::cout << "ERROR::program::linking_failed." << std::endl;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(_id);
}

unsigned int Shader::GetID() const {
    return _id;
}

void Shader::Use() const {
    glUseProgram(_id);
}

void Shader::SetBool(const std::string& name, bool value) const {
    this->Use();
    glUniform1i(glGetUniformLocation(_id, name.c_str()), int(value));
}

void Shader::SetFloat(const std::string& name, float value) const {
    this->Use();
    glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::SetInt(const std::string& name, int value) const {
    this->Use();
    glUniform1i(glGetUniformLocation(_id, name.c_str()), int(value));
}

std::string Shader::ReadFile(const char* path) {
    std::string str;
    std::ifstream stream;
    stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        stream.open(path);
        std::stringstream ss;
        ss << stream.rdbuf();
        stream.close();
        str = ss.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR:shader::file not successfully read." << std::endl;
    }
    return str;
}

unsigned int Shader::CreateShader(const char* str, ShaderType type) {
    int success{0};
    char info_log[512] {};
    unsigned int shader{0};
    if (VERTEX == type) {
        shader = glCreateShader(GL_VERTEX_SHADER);
    } else if (FRAGMENT == type) {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    } else {
        assert(false && "shader type error.");
    }
    glShaderSource(shader, 1, &str, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(info_log), nullptr, info_log);
        std::cout << "ERROR::shader::<" << type << ">::compilation_failed." << std::endl;
    }
    return shader;
}
