
#include "camera/Camera.h"
#include "input/KeyboardHandler.h"

Camera::Camera() {}

Camera::~Camera() {}

void Camera::setSize(float frameBufferWidth, float frameBufferHeight) {

    m_projectionMatrix = glm::perspective(
        glm::radians(m_fov), 
        frameBufferWidth / frameBufferHeight, 
        0.01f, 
        500.0f
    );
    
    m_frameBuffer.create(frameBufferWidth, frameBufferHeight);

}

void Camera::bind() const {
    m_frameBuffer.bind();
}

void Camera::update(float time) {

    if(m_autoRotate) {

        // m_camRadius = glm::distance(m_position, m_lookAtPos);

        m_position.x = sin(time) * m_camRadius;
        m_position.z = cos(time) * m_camRadius;
        m_position.y = 1.0f;

    } else if (m_enableFreeCam) {

        float posDelta = m_speed; // * time.dt;
    
        if (Keyboard::shift)
            posDelta *= 5.0f;

        if (Keyboard::ctrl)
            posDelta *= 0.1f;

        if (Keyboard::w)
            m_position += front * posDelta;
        if (Keyboard::s)
            m_position -= front * posDelta;
        if (Keyboard::a)
            m_position -= right * posDelta;
        if (Keyboard::d)
            m_position += right * posDelta;

        int x, y;
        SDL_GetRelativeMouseState(&x, &y);

        m_eulerRotation.x += x * 0.5f;
        m_eulerRotation.y -= y * 0.5f;

        m_eulerRotation.y = std::clamp(m_eulerRotation.y, -89.5f, 89.5f);

        glm::vec3 _front;
        _front.x = cos(glm::radians(m_eulerRotation.x)) * cos(glm::radians(m_eulerRotation.y));
        _front.y = sin(glm::radians(m_eulerRotation.y));
        _front.z = sin(glm::radians(m_eulerRotation.x)) * cos(glm::radians(m_eulerRotation.y));

        front = glm::normalize(_front);
        right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
        up    = glm::normalize(glm::cross(right, front));

        m_lookAtPos = m_position + front;

        // m_viewMatrix = glm::lookAt(m_position, m_position + front, up);

    }
    
    m_viewMatrix = glm::lookAt(m_position, m_lookAtPos, vec3(0.0, 1.0, 0.0));
    
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}
