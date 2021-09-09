#include "config.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>

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


    /// 顶点着色器(Vertex Shader).
    const unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const unsigned int vertex_shader1 = glCreateShader(GL_VERTEX_SHADER);
    {
        /// 使用着色器语言(OpenGL Shading Language, GLSL)编写顶点着色器.
        const char* vertex_shader_source = "#version 330 core\n"
                                           "layout (location = 0) in vec3 aPos;\n"
                                           "void main()\n"
                                           "{\n"
                                           "  gl_Position = vec4(aPos, 1.0);\n"
                                           "}\n";

        const char* vertex_shader_source1 = "#version 330 core\n"
                                            "layout (location = 0) in vec3 aPos;\n"
                                            "layout (location = 1) in vec3 aColor;\n"
                                            "out vec3 array_color;\n"
                                            "void main()\n"
                                            "{\n"
                                            "   gl_Position = vec4(aPos, 1.0);\n"
                                            "   array_color = aColor;\n"
                                            "}\0";

        // 使用源码定义着色器.
        glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
        glShaderSource(vertex_shader1, 1, &vertex_shader_source1, nullptr);
        // 编译着色器.
        glCompileShader(vertex_shader);
        glCompileShader(vertex_shader1);
        // 检测编译是否成功.
        int success{0};
        char info_log[512] {};
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex_shader, sizeof(info_log), nullptr, info_log);
            std::cout << "ERROR::shader::vertex::compilation_failed\n" << info_log << std::endl;
        }
    }

    /// 片段着色器(Fragment Shader).
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragment_shader1 = glCreateShader(GL_FRAGMENT_SHADER);
    {
        /// 使用着色器语言GLSL编写片段着色器.
        const char* fragment_shader_source = "#version 330 core\n"
                                             "in vec4 vertex_color;\n"
                                             "uniform vec4 ufm_color;\n"
                                             "out vec4 fc;\n"
                                             "void main()\n"
                                             "{ fc = ufm_color; }\0";

        const char* fragment_shader_source1 = "#version 330 core\n"
                                              "in vec3 array_color;\n"
                                              "out vec4 frag_color;\n"
                                              "void main()\n"
                                              "{\n"
                                              "  frag_color = vec4(array_color, 1.0f);\n"
                                              "}\n\0";

        // 使用源码定义着色器.
        glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
        glShaderSource(fragment_shader1, 1, &fragment_shader_source1, nullptr);
        // 编译着色器.
        glCompileShader(fragment_shader);
        glCompileShader(fragment_shader1);
        int success{0};
        char info_log[512] {};
        //
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment_shader, sizeof(info_log), nullptr, info_log);
            std::cout << "ERROR::shader::fragment::compilation_failed\n" << info_log << std::endl;
        }
    }

    /// 着色器程序(Shader Program Object).
    const unsigned int shader_program = glCreateProgram();
    const unsigned int shader_program1 = glCreateProgram();
    {
        // 多个着色器合并, 最终链接成着色器程序对象.
        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, fragment_shader);
        glLinkProgram(shader_program);

        glAttachShader(shader_program1, vertex_shader1);
        glAttachShader(shader_program1, fragment_shader1);
        glLinkProgram(shader_program1);

        int success{0};
        char info_log[512] {};
        // 检测着色器程序状态.
        glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader_program, sizeof(info_log), nullptr, info_log);
            std::cout << "ERROR::program::link_failed.\n" << info_log << std::endl;
        }
        // 把着色器连接到程序对象后, 即可删除着色器对象.
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glDeleteShader(fragment_shader1);
    }

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
        glUseProgram(shader_program1);
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
