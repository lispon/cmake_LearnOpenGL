#include "starttextures.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


StartTextures::StartTextures()
    : _vao(0),
      _vbo(0),
      _ebo(0),
      _texture(0),
      _program1(0) {

}

StartTextures::~StartTextures() {

}

void StartTextures::ConstructData() {
    Shader shader1("./shader/texture_container.vs.glsl", "./shader/texture_container.fs.glsl");
    _program1 = shader1.GetID();

    const float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----              ---- 纹理坐标 ----
        0.5f,  0.5f,  0.0f,         1.0f,  0.0f,  0.0f,        1.0f,  1.0f,    // 右上
        0.5f, -0.5f,  0.0f,         0.0f,  1.0f,  0.0f,        1.0f,  0.0f,    // 右下
        -0.5f, -0.5f,  0.0f,         0.0f,  0.0f,  1.0f,        0.0f,  0.0f,   // 左下
        -0.5f,  0.5f,  0.0f,         1.0f,  1.0f,  0.0f,        0.0f,  1.0f    // 左上
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
        glBindTexture(GL_TEXTURE_2D, _texture);
        //
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //
        int width{0}, height{0}, nrChannels{0};
        const char* img_path = "./img/container.jpg";
        unsigned char* data = stbi_load(img_path, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture:" << img_path << std::endl;
        }
        stbi_image_free(data);
    }
}

void StartTextures::DestructData() {
    glDeleteTextures(1, &_texture);
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteProgram(_program1);
}

void StartTextures::Draw() {
    glBindTexture(GL_TEXTURE_2D, _texture);
    glBindVertexArray(_vao);

    glUseProgram(_program1);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
