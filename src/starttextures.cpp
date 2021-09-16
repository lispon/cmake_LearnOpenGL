#include "starttextures.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


StartTextures::StartTextures()
    : _shader(nullptr),
      _opacity(0.2f),
      _vao(0),
      _vbo(0),
      _ebo(0),
      _texture(0),
      _texture2(0) {

}

StartTextures::~StartTextures() {

}

void StartTextures::ConstructData() {
    _shader = std::make_shared<Shader>("./shader/texture_container.vs.glsl", "./shader/texture_container.fs.glsl");

    const float vertices[] {
        //     ---- 位置 ----       ---- 颜色 ----              ---- 纹理坐标 ----
        0.5f,  0.5f,  0.0f,         1.0f,  0.0f,  0.0f,        3.0f,  3.0f,    // 右上
        0.5f, -0.5f,  0.0f,         0.0f,  1.0f,  0.0f,        3.0f,  0.0f,    // 右下
        -0.5f, -0.5f,  0.0f,         0.0f,  0.0f,  1.0f,        0.0f,  0.0f,   // 左下
        -0.5f,  0.5f,  0.0f,         1.0f,  1.0f,  0.0f,        0.0f,  3.0f    // 左上
    };
    unsigned int indices[] {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 生成纹理.
    {
        glGenTextures(1, &_texture);
//        glActiveTexture(GL_TEXTURE0); // 默认激活0.
        glBindTexture(GL_TEXTURE_2D, _texture);
        //
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //
        int width{0}, height{0}, nrChannels{0};
        const char* img_path = "./img/container.jpg";
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(img_path, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture:" << img_path << std::endl;
        }
        stbi_image_free(data);

        glGenTextures(1, &_texture2);
        glBindTexture(GL_TEXTURE_2D, _texture2);
        //
        float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        const char* img_path2 = "./img/awesomeface.png";
        stbi_set_flip_vertically_on_load(true);
        data = stbi_load(img_path2, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture:" << img_path2 << std::endl;
        }
        stbi_image_free(data);
    }

    //
    _shader->SetInt("texture1", 0);
    _shader->SetInt("texture2", 1);

    SetTextureOpacity(true);
}

void StartTextures::DestructData() {
    glDeleteTextures(1, &_texture);
    glDeleteTextures(1, &_texture2);
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
}

void StartTextures::Draw() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _texture2);

    _shader->Use();
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void StartTextures::ProcessInput() {
    bool up = true;
    if (glfwGetKey(_glfw_window, GLFW_KEY_UP) == GLFW_PRESS) {
        up = true;
    } else if (glfwGetKey(_glfw_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        up = false;
    } else {
        return;
    }
    SetTextureOpacity(up);
}

void StartTextures::SetTextureOpacity(bool up) {
    const static float step = 0.001f;
    if (up) {
        _opacity += step;
    } else {
        _opacity -= step;
    }

    if (_opacity < 0) {
        _opacity = 0;
    } else if (1 < _opacity) {
        _opacity = 1;
    }
    _shader->SetFloat("opacity", _opacity);
    std::cout << "opacity:" << _opacity << std::endl;
}
