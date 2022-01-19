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

#endif // GLM_DEMO_H
