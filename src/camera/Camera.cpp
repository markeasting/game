#include "common.h"
#include "camera/Camera.h"
#include "input/Mouse.h"
#include "util/log.h"
#include "event/EventEmitter.h"

Camera::Camera() {

    Events.on("click", [&] (GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            this->autoRotate = false;
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            this->autoRotate = true;
        }
    });

}

Camera::~Camera() {}

void Camera::update(const float& time) {

    Mouse &mouse = Mouse::instance();

    if(this->autoRotate) {
        this->_autoRotate(time);
    } else {

        float posDelta = this->speed; // * time.dt;

        // @TODO store pressed keys in Game
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
            posDelta *= 3;
        if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) 
            posDelta *= 0.333;

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            this->position += this->front * posDelta;
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            this->position -= this->front * posDelta;
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            this->position -= this->right * posDelta;
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            this->position += this->right * posDelta;

        glm::vec2 mouseDelta = mouse.getDelta();

        this->eulerRotation.x += mouseDelta.x;
        this->eulerRotation.y -= mouseDelta.y;

        if (this->eulerRotation.y > 89.5f)
            this->eulerRotation.y = 89.5f;
        if (this->eulerRotation.y < -89.5f)
            this->eulerRotation.y = -89.5f;

        glm::vec3 _front;
        _front.x = cos(glm::radians(this->eulerRotation.x)) * cos(glm::radians(this->eulerRotation.y));
        _front.y = sin(glm::radians(this->eulerRotation.y));
        _front.z = sin(glm::radians(this->eulerRotation.x)) * cos(glm::radians(this->eulerRotation.y));

        this->front = glm::normalize(_front);
        this->right = glm::normalize(glm::cross(this->front, glm::vec3(0.0f, 1.0f, 0.0f)));
        this->up    = glm::normalize(glm::cross(this->right, this->front));

        this->viewMatrix = glm::lookAt(this->position, this->position + this->front, this->up);

    }
    
    this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}

void Camera::setProjection(float frameBufferWidth, float frameBufferHeight) {
    this->frameBufferWidth = frameBufferWidth;
    this->frameBufferHeight = frameBufferHeight;
    this->projectionMatrix = glm::perspective(glm::radians(this->fov), (float) this->frameBufferWidth / (float) this->frameBufferHeight, 0.01f, 50.0f);
}

void Camera::_autoRotate(const float& time) {
    
    this->lookAtPos = glm::vec3(0.0f, 1.0f, 0.0f);

    const float camRadius = glm::distance(this->position, this->lookAtPos);

    this->position.x = sin(time) * camRadius;
    this->position.z = cos(time) * camRadius;
    this->position.y = 1.0f;

    this->viewMatrix = glm::lookAt(this->position, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}
