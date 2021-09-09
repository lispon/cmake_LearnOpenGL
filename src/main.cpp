#include "config.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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


    /// 顶点着色器(Vertex Shader).
    const unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    {
        /// 使用着色器语言(OpenGL Shading Language, GLSL)编写顶点着色器.
        const char* vertex_shader_source = "#version 330 core\n"
                                           "layout (location = 0) in vec3 aPos;\n"
                                           "void main()\n"
                                           "{\n"
                                           "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                           "}\0";
        // 使用源码定义着色器.
        glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
        // 编译着色器.
        glCompileShader(vertex_shader);
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
    {
        /// 使用着色器语言GLSL编写片段着色器.
        const char* fragment_shader_source = "#version 330 core\n"
                                             "out vec4 FragColor;\n"
                                             "void main()\n"
                                             "{\n"
                                             "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                             "}\0";
        // 使用源码定义着色器.
        glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
        // 编译着色器.
        glCompileShader(fragment_shader);
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
    {
        // 多个着色器合并, 最终链接成着色器程序对象.
        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, fragment_shader);
        glLinkProgram(shader_program);
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
    }

    /// 顶点数组对象(Vertex Array Object, VAO).
    unsigned int vao{0};
    /// 顶点缓存对象(Vertex Buffer Objects, VBO).
    unsigned int vbo{0};
    {
        // 右手坐标系坐标.
        const float vertices[] = { -0.5f, -0.5f, 0.0f,
                                   0.5f, -0.5f, 0.0f,
                                   0.0f, 0.5f, 0.0f,
                                 };
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(vao);

        // 绑定缓冲. 可以同时绑定多个不同类型的缓冲.
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // 将用户定义的vertices数据复制到绑定缓冲中.
        // GL_STATIC_DRAW 数据不会或几乎不会改变.
        // GL_DYNAMIC_DRAW 数据会改变很多.
        // GL_STREAM_DRAW 数据每次绘制时都会改变.
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // 设置顶点属性指针.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

    }

    // uncomment this call to draw in wireframe polygons.
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        // 绘制图形.
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // 没有必要每次都解除绑定.
        glBindVertexArray(0);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
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
