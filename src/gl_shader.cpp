#include "gl_shader.h"

GLShader::GLShader(std::string vertex_path, std::string fragment_path)
    :_id(0)
{
    std::string vertex_code, fragment_code;
    std::ifstream vs_file, fs_file;
    // ensure ifstream objects can throw exceptions.
    vs_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fs_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vs_file.open(vertex_path);
        fs_file.open(fragment_path);
        std::stringstream vs_stream, fs_stream;
        vs_stream << vs_file.rdbuf();
        fs_stream << fs_file.rdbuf();
        vs_file.close();
        fs_file.close();
        vertex_code = vs_stream.str();
        fragment_code = fs_stream.str();
    }  catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what()
                  << "\n error file:" << vertex_path << fragment_path << std::endl;
    }

    const unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    const char* vc = vertex_code.c_str();
    glShaderSource(vertex, 1, &vc, nullptr);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");

    const unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fc = fragment_code.c_str();
    glShaderSource(fragment, 1, &fc, nullptr);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");

    const unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    CheckCompileErrors(program, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

unsigned int GLShader::GetID() const {
    return _id;
}

void GLShader::CheckCompileErrors(unsigned int shader, std::string type) {
    int success = 0;
    char info_log[1024];
    if("PROGRAM" != type) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, sizeof(info_log), nullptr, info_log);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << info_log
                      << "\n -- -------------------------------- --" << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shader, sizeof(info_log), nullptr, info_log);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << info_log
                      << "\n -- -------------------------------- --" << std::endl;
        }
    }
}
