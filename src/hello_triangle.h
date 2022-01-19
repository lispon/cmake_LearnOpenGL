#ifndef HELLO_TRIANGLE_H
#define HELLO_TRIANGLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif


#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int Triangle(int type) {
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
                                       "layout (location = 1) in vec3 aColor;\n"
                                       "layout (location = 2) in vec2 aTexCoord;\n"
                                       "out vec4 vertexColor;\n"
                                       "out vec2 TexCoord;\n"
                                       "void main()\n"
                                       "{\n"
                                       "    gl_Position = vec4(aPos, 1.0);\n"
                                       "    vertexColor = vec4(aColor, 1.0);\n"
                                       "    TexCoord = aTexCoord;\n"
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
                                         "in vec4 vertexColor;\n"
                                         "in vec2 TexCoord;\n"
                                         "uniform vec4 ourColor;\n"
                                         "uniform sampler2D ourTexture;\n"
                                         "uniform sampler2D ourTexture1;\n"
                                         "void main()\n"
                                         "{\n"
                                         "    FragColor = mix(texture(ourTexture, TexCoord) * vertexColor, texture(ourTexture1, TexCoord), 0.2);\n"
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


    unsigned int texture[] = { 0, 0 };
    unsigned int vao = { 0 };
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    {
        glGenTextures(sizeof(texture), texture);
        // 默认自动激活纹理单元0
//        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);

        int img_width = 0, img_height = 0, img_channels = 0;
        const char* img_name = "img/container.jpg";
        unsigned char* img_data = stbi_load(img_name, &img_width, &img_height, &img_channels, 0);
        if(img_data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture img:" << img_name << std::endl;
        }
        stbi_image_free(img_data);

//        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        img_width = img_height = img_channels = 0;
        const char* img_name2 = "img/awesomeface.png";
        unsigned char* img_data2 = stbi_load(img_name2, &img_width, &img_height, &img_channels, 0);
        if(img_data2) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data2);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture img:" << img_name2 << std::endl;
        }
        stbi_image_free(img_data2);


        glUseProgram(shader_program);
        // don't forget to acctivate/use the shader program before setting uniforms.
        glUniform1i(glGetUniformLocation(shader_program, "ourTexture"), 0);
        glUniform1i(glGetUniformLocation(shader_program, "ourTexture1"), 1);


        const float vertices[] = {
            //      位置                      颜色                  纹理
            -0.5f, -0.5f, 0.0f,         1.0f, 0.0f, 0.0f,       0.0f, 0.0f,     // 左下角
             0.5f, -0.5f, 0.0f,         1.0f, 1.0f, 0.0f,       2.0f, 0.0f,     // 右下角
             0.5f,  0.5f, 0.0f,         1.0f, 1.0f, 1.0f,       2.0f, 2.0f,     // 右上角
            -0.5f,  0.5f, 0.0f,         0.0f, 1.0f, 1.0f,       0.0f, 2.0f,     // 左上角
             0.0f,  0.5f, 0.0f,         0.0f, 0.0f, 1.0f,       0.0f, 2.0f,     // 上中点
        };


        unsigned int vbo = { 0 };
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        if(1 == type || 2 == type) {
            unsigned int ebo = { 0 };
            glGenBuffers(1, &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        }
        if(1 == type) {
            const unsigned int indices_triangle[] = {
                0, 1, 4,
            };
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_triangle), indices_triangle, GL_STATIC_DRAW);
        } else if(2 == type) {
            const unsigned int indices[] = {
                0, 1, 2,                    // 右下三角形
                0, 2, 3,                    // 左上三角形
            };
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        } else if(3 == type) {
            //
        } else {
            glfwTerminate();
            assert(false);
            return -1;
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              reinterpret_cast<void*>(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    glUseProgram(shader_program);

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        {
            double time_value = glfwGetTime();
            float green_value = (sin(time_value) / 2.0f) + 0.5f;
            int vertex_color_location = glGetUniformLocation(shader_program, "ourColor");
#if 0
            glUniform4f(vertex_color_location, 0.0f, green_value, 0.0f, 1.0f);
#else
            const float fv[] = {0.0f, green_value, 0.0f, 1.0f};
            glUniform1fv(vertex_color_location, 4, fv);
#endif
//            int texture_sampler_location = glGetUniformLocation(shader_program, "ourTexture");

        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        glBindVertexArray(vao);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if(1 == type) {
//        glDrawArrays(GL_TRIANGLES, 0, 3);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        } else if(2 == type) {
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        } else if(3 == type) {
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &vao);

    glfwTerminate();
    return 0;
}


int HelloTriangle() {
    return Triangle(1);
}

int HelloRectangle() {
    return Triangle(2);
}

int HelloVertexColor() {
    return Triangle(3);
}

#endif // HELLO_TRIANGLE_H
