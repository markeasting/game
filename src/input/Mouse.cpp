
#include "input/Mouse.h"
#include "event/EventEmitter.h"

Mouse& Mouse::instance() {
    static Mouse instance;
    return instance;
}

glm::vec2 Mouse::getDelta() {

    double dx = m_sensitivity * (m_pos.x - m_lastPos.x);
    double dy = m_sensitivity * (m_pos.y - m_lastPos.y);

    return glm::vec2(dx, dy);
}

void Mouse::update(GLFWwindow* window) {

    double _absolutePosX;
    double _absolutePosY;

    int _windowWidth;
    int _windowHeight;

    glfwGetCursorPos(window, &_absolutePosX, &_absolutePosY);
    glfwGetWindowSize(window, &_windowWidth, &_windowHeight);

    m_absolutePos.x = (float) _absolutePosX;
    m_absolutePos.y = (float) _absolutePosY;

    m_lastPos.x = m_pos.x;
    m_lastPos.y = m_pos.y;

    m_pos.x = (m_absolutePos.x / _windowWidth) * 2 - 1;
    m_pos.y = (m_absolutePos.y / _windowHeight) * 2 - 1;

}
