#pragma once

#include "common.h"

class Mouse {
public:

    float m_sensitivity = 75.0f;

    glm::vec2 m_pos = glm::vec2(0.0f);

	static Mouse& instance();

	Mouse(const Mouse&) = delete;
    ~Mouse() = default;

    glm::vec2 getDelta();

    void update(GLFWwindow* window);

private:

    Mouse() = default;

    glm::vec2 m_lastPos = { 0, 0 };
    glm::vec2 m_absolutePos = glm::vec2(0.0f);

};
