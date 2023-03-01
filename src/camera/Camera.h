#pragma once

#include "common.h"
#pragma once

#include "core/Object3D.h"

class Camera : public Object3D {
public:

    float frameBufferWidth = 0;
    float frameBufferHeight = 0;
    
    float speed = 0.05; // 3.0f; (with dt)
    float fov = 70; // g_settings.fov;
    // float zoomspeed = 0.05f;

    bool autoRotate = true;

    glm::vec3 lookAtPos = glm::vec3(0.0f);
    glm::vec3 eulerRotation = glm::vec3(-90.0f, 0.0f, 0.0f);

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    glm::mat4 viewProjectionMatrix = glm::mat4(1.0f);

    Camera();
    ~Camera();

    void update(const float& time);
    void setProjection(float frameBufferWidth, float frameBufferHeight);

private:
	GLFWwindow* window; // @TODO remove, this should not be here!

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    
    void _autoRotate(const float& time);

};
