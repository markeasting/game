#pragma once

#include "common.h"

#include "core/Object3D.h"
#include "gfx/FrameBuffer.h"

class Camera : public Object3D {
public:
    
    float m_speed = 0.05; // 3.0f; (with dt)
    float m_fov = 70; // g_settings.fov;
    // float zoomspeed = 0.05f;

    bool m_autoRotate = true;

    glm::vec3 m_lookAtPos = glm::vec3(0.0f);
    glm::vec3 m_eulerRotation = glm::vec3(-90.0f, 0.0f, 0.0f);

    glm::mat4 m_viewMatrix = glm::mat4(1.0f);
    glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
    glm::mat4 m_viewProjectionMatrix = glm::mat4(1.0f);

    FrameBuffer m_frameBuffer; // @TODO use unique_ptr here?

    Camera();
    ~Camera();

    void bind() const;

    void update(float time);
    void setSize(float frameBufferWidth, float frameBufferHeight);

private:
	GLFWwindow* window; // @TODO remove, this should not be here!

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    
    void _autoRotate(float time);

};
