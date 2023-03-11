#pragma once

#include "common.h"
#include <vector>

class Object3D {
public:
    
    // const char* name = "";
    // const char* type = "Object3D"; // @TODO make enum or use instanceof?

    bool m_useProjectionMatrix = true;

    // std::weak_ptr<Object3D> parent;
    Object3D* parent = nullptr; // @TODO use weak_ptr?
    std::vector<Ref<Object3D>> children = {};

    // @TODO convert these to chaining pattern
    // https://levelup.gitconnected.com/how-to-implement-method-chaining-in-c-3ec9f255972a
    void add(Ref<Object3D> object);
    
    void setPosition(glm::vec3 position);

    void setRotation(const glm::vec3& euler);
    void setRotation(const glm::quat& rotation);
    
    void setScale(const float& uniformScale);
    void setScale(const glm::vec3& scale);
    
    void translate(glm::vec3 translation);
    void rotate(float angle, glm::vec3 direction);

    glm::vec3 getPosition();
    glm::quat getRotation();
    glm::vec3 getScale();

    glm::mat4 getWorldPositionMatrix();

protected:

    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat m_rotation = glm::quat(1.0f, 0, 0, 0);
    glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

private:

    bool m_worldPosMatrixNeedsUpdate = true;
    glm::mat4 m_worldPositionMatrix = glm::mat4(1.0f);

};

// using Object3DRef = Ref<Object3D>;
