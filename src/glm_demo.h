#ifndef GLM_DEMO_H
#define GLM_DEMO_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void TestRotate(glm::mat4& m4) {
    // 绕 z 轴旋转 45°.
    m4 = glm::rotate(m4, glm::radians(45.0f), glm::vec3(.0f, .0f, 1.0f));
}

void TestTranslate(glm::mat4& m4) {
    // 平移 (0, 1, 0)
    m4 = glm::translate(m4, glm::vec3(.0f, 1.0f, .0f));
}

void TestScale(glm::mat4& m4) {
    // 缩放 2 倍.
    m4 = glm::scale(m4, glm::vec3(2.0f, 2.0f, 2.0f));
}

void TestCoutV3(const glm::vec4& v4) {
    std::cout << "v4:" << v4.x << "," << v4.y << "," << v4.z << std::endl;
}

void testRunOrder() {
    glm::vec4 v4_1, v4_2;
    v4_1 = v4_2 = { 1.0f, 1.0f, .0f , 1.0f };

    glm::mat4 m4_1(1.0f), m4_2(1.0f);

    std::cout << "translate, rotate:" << std::endl;
    TestTranslate(m4_1);
    TestRotate(m4_1);
    v4_1 = m4_1 * v4_1;
    TestCoutV3(v4_1);

    std::cout << "rotate, translate:" << std::endl;
    TestRotate(m4_2);
    TestTranslate(m4_2);
    v4_2 = m4_2 * v4_2;
    TestCoutV3(v4_2);

    std::cout << "#################################" << std::endl;
}

void HelloGlm() {
    testRunOrder();

    std::cout << "start:hello glm:-------------" << std::endl;
    glm::vec4 v4(1.0f, 2.0f, 3.0f, 1.0f);
    glm::mat4 m4(1.0f);
    // 顺序, 先缩放, 然后旋转, 最后平移.
    // 实际执行操作的顺序, 与阅读顺序相反. 因为最后的操作首先与向量相乘.
    // 具体, 还需要实际测试并理解. 见 testRunOrder 函数.

    // 平移
    m4 = glm::translate(m4, glm::vec3(1.0f, 1.0f, 0.0f));
    // 旋转
    m4 = glm::rotate(m4, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0f));
    // 缩放
    m4 = glm::scale(m4, glm::vec3(0.5f, 0.5f, 0.5f));

    v4 = m4 * v4;
    std::cout << "after_translate:" << v4.x << "," << v4.y << "," << v4.z << "," << v4.w << std::endl;
    std::cout << "e n d:  hello glm:-------------" << std::endl;
}

void CustomLookAt() {
    const glm::vec4 test_v4(1.1f, 2.2f, 3.3f, 4.4f);

    const glm::vec3 camera_pos(.0f, .0f, 3.0f);     // 摄像机位置.
    const glm::vec3 camera_target(.0f, .0f, .0f);   // 观察目标中心点.
    const glm::vec3 camera_up(.0f, 1.0f, .0f);      // 上向量.

    const glm::mat4 lookat_view = glm::lookAt(camera_pos, camera_target, camera_up);
    auto v4 = lookat_view * test_v4;
    std::cout << "lookat_view:" << v4.x << "," << v4.y << "," << v4.z << "," << v4.w << std::endl;

    // 自己实现 lookat 矩阵.
    {
        glm::vec3 camera_direction = glm::normalize(camera_pos - camera_target);
        glm::vec3 up = glm::vec3(.0f, 1.0f, .0f);
        glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_direction));
        glm::vec3 camera_up = glm::cross(camera_direction, camera_right);

        glm::mat4 clookat_view(1.0f), l4(1.0f), r4(1.0f);

        // l4
        l4[0][0] = camera_right.x;
        l4[0][1] = camera_right.y;
        l4[0][2] = camera_right.z;

        l4[1][0] = camera_up.x;
        l4[1][1] = camera_up.y;
        l4[1][2] = camera_up.z;

        l4[2][0] = camera_direction.x;
        l4[2][1] = camera_direction.y;
        l4[2][2] = camera_direction.z;

        // r4
        r4[3][0] = -camera_pos.x;
        r4[3][1] = -camera_pos.y;
        r4[3][2] = -camera_pos.z;

        clookat_view = l4 * r4;
        auto v4 = clookat_view * test_v4;
        std::cout << "clookat_view:" << v4.x << "," << v4.y << "," << v4.z << "," << v4.w << std::endl;
    }
}

#endif // GLM_DEMO_H
