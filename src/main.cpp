#include "config.h"
#include "startshader.h"
#include "starttextures.h"

#include "hello_window.h"
#include "hello_triangle.h"

#include "gflags/gflags.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <functional>

static bool ValidateExe(const char* flagname, uint32_t value) {
    std::function<void(void)> fun_exe;
    switch (value) {
    case 0:{
        fun_exe = &HelloWindow;
        break;
    }
    case 1: {
        fun_exe =&HelloTriangle;
        break;
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
    gflags::SetUsageMessage("usageMessage.");
    gflags::SetVersionString("0.0.1");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    std::cout << "gflags-curent-exe:" << FLAGS_exe << std::endl;


//    StartShader wgt1;
//    wgt1.Show();

//    StartTextures wgt;
//    wgt.Show();

    return 0;
}
