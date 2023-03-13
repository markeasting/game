#include "common.h"
#include "camera/Camera.h"
#include "input/Mouse.h"
#include "event/EventEmitter.h"

Camera::Camera() {

    Events.on("click", [&] (GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            m_autoRotate = false;
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_autoRotate = true;
        }
    });

}

Camera::~Camera() {}

void Camera::bind() const {
    m_frameBuffer.bind();
}

void Camera::update(float time) {

    Mouse &mouse = Mouse::instance();

    if(m_autoRotate) {
        _autoRotate(time);
    } else {

        float posDelta = m_speed; // * time.dt;

        // @TODO store pressed keys in Game
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
            posDelta *= 3;
        if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) 
            posDelta *= 0.333;

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            m_position += front * posDelta;
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            m_position -= front * posDelta;
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            m_position -= right * posDelta;
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            m_position += right * posDelta;

        glm::vec2 mouseDelta = mouse.getDelta();

        m_eulerRotation.x += mouseDelta.x;
        m_eulerRotation.y -= mouseDelta.y;

        if (m_eulerRotation.y > 89.5f)
            m_eulerRotation.y = 89.5f;
        if (m_eulerRotation.y < -89.5f)
            m_eulerRotation.y = -89.5f;

        glm::vec3 _front;
        _front.x = cos(glm::radians(m_eulerRotation.x)) * cos(glm::radians(m_eulerRotation.y));
        _front.y = sin(glm::radians(m_eulerRotation.y));
        _front.z = sin(glm::radians(m_eulerRotation.x)) * cos(glm::radians(m_eulerRotation.y));

        front = glm::normalize(_front);
        right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
        up    = glm::normalize(glm::cross(right, front));

        m_viewMatrix = glm::lookAt(m_position, m_position + front, up);

    }
    
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

void Camera::setSize(float frameBufferWidth, float frameBufferHeight) {
    m_projectionMatrix = glm::perspective(glm::radians(m_fov), (float) frameBufferWidth / (float) frameBufferHeight, 0.01f, 50.0f);

    m_frameBuffer.create(frameBufferWidth, frameBufferHeight);

}

void Camera::_autoRotate(float time) {
    
    m_lookAtPos = glm::vec3(0.0f, 1.0f, 0.0f);

    const float camRadius = glm::distance(m_position, m_lookAtPos);

    m_position.x = sin(time) * camRadius;
    m_position.z = cos(time) * camRadius;
    m_position.y = 1.0f;

    m_viewMatrix = glm::lookAt(m_position, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}
