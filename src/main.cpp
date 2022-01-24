#include "config.h"

#include "hello_window.h"
#include "hello_triangle.h"

#include "glm_demo.h"

#include "gflags/gflags.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <functional>

static bool ValidateExe(const char* flagname, uint32_t value) {
    std::function<void(void)> fun_exe;
    switch (value) {
    case 0:{
        // 显示默认空窗口.
        fun_exe = &HelloWindow;
        break;
    }
    case 1: {
        // 使用索引缓冲对象(EBO)显示三角形
        fun_exe =&HelloTriangle;
        break;
    }
    case 2: {
        // 使用索引缓冲对象(EBO)显示矩形.
        fun_exe = &HelloRectangle;
        break;
    }
    case 3: {
        // 显示三角形.
        fun_exe = &HelloVertexColor;
        break;
    }
    case 4: {
        // 显示 3D.
        fun_exe = &Hello3D;
        break;
    }
    case 5: {
        // 使用键盘中 wasd 控制摄像头.
        fun_exe = &HelloWASD;
    }
    default: {
        fun_exe = [value](){ std::cout << "not support 'exe' value:" << value << std::endl; };
        assert(false);
    }
    }

    std::cout << "validateExe: " << flagname << "->" << value << std::endl;
    fun_exe();
    return true;
}
DEFINE_uint32(exe, 0, "executable index.");
DEFINE_validator(exe, &ValidateExe);


int main(int argc, char* argv[]) {
    std::cout << "VERSION:" << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;

//    HelloGlm();
//    CustomLookAt();

    gflags::SetUsageMessage("usageMessage.");
    gflags::SetVersionString("0.0.1");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    std::cout << "gflags-current-exe:" << FLAGS_exe << std::endl;

//    StartShader wgt1;
//    wgt1.Show();

//    StartTextures wgt;
//    wgt.Show();

    return 0;
}
