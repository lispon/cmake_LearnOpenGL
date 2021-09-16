#include "openglwindowbase.h"

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    (void)window;
    glViewport(0, 0, width, height);
}

int OpenGLWindowBase::_unique = 0;

OpenGLWindowBase::OpenGLWindowBase() {
    InitWindow();
}

OpenGLWindowBase::~OpenGLWindowBase() {
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void OpenGLWindowBase::ProcessInput() {
    if (glfwGetKey(_glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        std::cout << "custom close::ESC key pressed." << std::endl;
        glfwSetWindowShouldClose(_glfw_window, true);
    }
}

void OpenGLWindowBase::Show() {
    ConstructData();

    while (!glfwWindowShouldClose(_glfw_window)) {
        ProcessInput();

        Draw();

        glfwSwapBuffers(_glfw_window);
        glfwPollEvents();
    }

    DestructData();
}

bool OpenGLWindowBase::InitWindow() {
    //
    assert(0 == _unique);
    ++_unique;


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

    bool ok = false;

    // glfw window creation
    // --------------------
    _glfw_window = glfwCreateWindow(_width, _height, "cmake_LearnOpenGL", nullptr, nullptr);
    if (_glfw_window) {
        glfwMakeContextCurrent(_glfw_window);
        glfwSetFramebufferSizeCallback(_glfw_window, FrameBufferSizeCallback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        ok = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (ok) {
            // opengl支持的顶点属性个数, 最小且一般为16.
            int nrAttributes;
            glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
            std::cout << "vertex attr max: " << nrAttributes << std::endl;
        } else {
            std::cout << "Failed to initialize GLAD." << std::endl;
        }
    } else {
        ok = false;
        std::cout << "Failed to create GLFW window." << std::endl;
    }
    return ok;
}
