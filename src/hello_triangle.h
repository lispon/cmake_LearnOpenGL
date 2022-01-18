#ifndef HELLO_TRIANGLE_H
#define HELLO_TRIANGLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <iostream>

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int HelloTriangle() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "triangle", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

    if(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Successed to initialize GLAD." << std::endl;
    } else {
        std::cout << "Failed to initialize GLAD." << std::endl;
        glfwTerminate();
        return -1;
    }

    unsigned int vertex_shader = { 0 };
    const char* vertex_shader_source = "#version 330 core\n"
                                       "layout (location = 0) in vec3 aPos;\n"
                                       "void main()\n"
                                       "{\n"
                                       "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                       "}\n";
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    int vertex_shader_success = { 0 };
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_shader_success);
    if(!vertex_shader_success) {
        char vertex_shader_info_log[512]{};
        glGetShaderInfoLog(vertex_shader, 512, nullptr, vertex_shader_info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertex_shader_info_log << std::endl;
    }

    unsigned int fragment_shader = { 0 };
    const char* fragment_shader_source = "#version 330 core\n"
                                         "out vec4 FragColor;\n"
                                         "void main()\n"
                                         "{\n"
                                         "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                         "}\n";
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);

    unsigned int shader_program = { 0 };
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    int shader_program_success = { 0 };
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &shader_program_success);
    if(!shader_program_success) {
        char shader_program_info_log[512];
        glGetProgramInfoLog(shader_program, 512, nullptr, shader_program_info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << shader_program_info_log << std::endl;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);



    const float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    unsigned int vao = { 0 };
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    {
        unsigned int vbo = { 0 };
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

#endif // HELLO_TRIANGLE_H
