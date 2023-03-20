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
    float m_camRadius = 10.0f;

    glm::vec3 m_lookAtPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_eulerRotation = glm::vec3(-90.0f, 0.0f, 0.0f);

    glm::mat4 m_viewMatrix = glm::mat4(1.0f);
    glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
    glm::mat4 m_viewProjectionMatrix = glm::mat4(1.0f);

    FrameBuffer m_frameBuffer; // @TODO use unique_ptr here?

    Camera();
    ~Camera();

    void setSize(float frameBufferWidth, float frameBufferHeight);

    // glm::vec3 getOrientation(); // @TODO use in audio listener setOrientation()
    inline glm::vec3 getForward() { return front; }
    inline glm::vec3 getUp() { return up; }
    inline glm::vec3 getRight() { return right; }

    void bind() const;
    void update(float time);


private:

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

};
