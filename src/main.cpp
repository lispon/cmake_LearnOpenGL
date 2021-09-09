#include "config.h"
#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>
#include <memory>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    std::cout << "VERSION:" << VERSION_MAJOR << "." << VERSION_MINOR << std::endl;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    // 指定opengl版本为3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 明确指定使用opengl的核心模式(Core-profile)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // opengl支持的顶点属性个数, 最小且一般为16.
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "vertex attr max: " << nrAttributes << std::endl;

    auto shader1 = std::make_shared<Shader>
                   ("./shader/rect.vs", "./shader/rect.fs");
    const auto shader_program = shader1->GetID();
    auto shader2 = std::make_shared<Shader>
                   ("./shader/tri.vs", "./shader/tri.fs");
    const auto shader_program2 = shader2->GetID();


    /// 顶点数组对象(Vertex Array Object, VAO).
    unsigned int vao{0};
    unsigned int vao1{0};
    /// 顶点缓冲对象(Vertex Buffer Objects, VBO).
    unsigned int vbo{0};
    unsigned int vbo1{0};
    /// 索引缓冲对象(Element Buffer Objects, ebo; 也称Index Buffer Objects, ibo).
    unsigned int ebo{0};
    {
        // 右手坐标系坐标.
        const float vertices[] { -0.5f, -0.5f, 0.0f, // 左下角.
                                 0.5f, -0.5f, 0.0f,  // 右下角.
                                 0.0f, 0.5f, 0.0f,   // 上中心.
                                 -0.5f, 0.5f, 0.0f,  // 左上角.
                                 0.5f, 0.5f, 0.0f,   // 右上角.
                                 0.0f, -0.5f, 0.0f,  // 下中心.
                               };

        const unsigned int indices[] {
            0, 1, 2,  // 中心三角形.
            0, 2, 3,  // 左上三角形.
            1, 2, 4,  // 右上三角形.
            0, 1, 3,  // 坐下三角形.
            0, 1, 4,  // 右下三角形.
        };

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        // bind the Vertex Array Object first,
        // then bind and set vertex buffer(s),
        // and then configure vertex attributes(s).
        {
            glBindVertexArray(vao);

            // 绑定缓冲. 可以同时绑定多个不同类型的缓冲.
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            // 将用户定义的vertices数据复制到绑定缓冲中.
            // GL_STATIC_DRAW 数据不会或几乎不会改变.
            // GL_DYNAMIC_DRAW 数据会改变很多.
            // GL_STREAM_DRAW 数据每次绘制时都会改变.
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // 复制我们的索引数组到一个索引缓冲中. 此时, vao中会保存当前索引缓冲对象的绑定状态.
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // 设置顶点属性指针.
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
        }

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex
        // attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO,
        // but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray
        // anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        //                        位置               颜色
        const float vertices1[] { -0.5f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f,  // 左上角.
                                  0.5f, 0.5f, 0.0f,     0.0f, 1.0f, 0.0f,  // 右上角.
                                  0.0f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,  // 下中心.
                                };
        glGenVertexArrays(1, &vao1);
        glGenBuffers(1, &vbo1);

        glBindVertexArray(vao1);
        glBindBuffer(GL_ARRAY_BUFFER, vbo1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

        // 位置属性
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // 颜色属性
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    // 线框模式(Wireframe Mode).
    // GL_POINT, GL_LINE, GL_FILL. 三种模式, 分别为点线面, 其中默认为GL_FILL
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // 绘制图形.
        glUseProgram(shader_program);

        // 更新uniform颜色ufm_color的值.
        float time_value = glfwGetTime();
        float red_value = (sin(time_value) / 2.0f) + 0.5f;
        int vertex_color_location = glGetUniformLocation(shader_program, "ufm_color");
        if (-1 == vertex_color_location) {
            std::cout << "ERROR::not found uniform color::ufm_color." << std::endl;
        } else {
            glUniform4f(vertex_color_location, red_value, 0.0f, 0.0f, 1.0f);
        }

        // 绑定vao(顶点数组对象)时, 会自动绑定ebo(索引缓冲对象).
        glBindVertexArray(vao);
        // seeing as we only have a single VAO there's no need to bind it every time,
        // but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0);
        // 没有必要每次都解除绑定.
//        glBindVertexArray(0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(shader_program2);
        int offset_x_location = glGetUniformLocation(shader_program2, "offset_x");
        if (-1 == offset_x_location) {
            std::cout << "ERROR::UNIFORM::offset_x_not_found." << std::endl;
        } else {
            glUniform1f(offset_x_location, 0.5f);
        }
        glBindVertexArray(vao1);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shader_program);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        std::cout << "ESC key pressed." << std::endl;
        glfwSetWindowShouldClose(window, true);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
