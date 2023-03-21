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

    vec3 m_lookAtPos = vec3(0.0f, 0.0f, 0.0f);
    vec3 m_eulerRotation = vec3(-90.0f, 0.0f, 0.0f);

    mat4 m_viewMatrix = mat4(1.0f);
    mat4 m_projectionMatrix = mat4(1.0f);
    mat4 m_viewProjectionMatrix = mat4(1.0f);

    FrameBuffer m_frameBuffer; // @TODO use unique_ptr here?

    Camera();
    ~Camera();

    void setSize(float frameBufferWidth, float frameBufferHeight);

    // vec3 getOrientation(); // @TODO use in audio listener setOrientation()
    inline vec3 getForward() { return front; }
    inline vec3 getUp() { return up; }
    inline vec3 getRight() { return right; }

    void bind() const;
    void update(float time);


private:

    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 front = vec3(0.0f, 0.0f, -1.0f);
    vec3 right = vec3(1.0f, 0.0f, 0.0f);

};
