
#include "engine/core/Object3D.h"
// #include <type_traits>

void Object3D::add(Ref<Object3D> object) {

    // static_assert(std::is_base_of_v<Object3D, T>);
    assert(object.get() != this);

    // if (object->parent) {
    //     // @TODO remove from parent
    // }

    object->m_parent = this;
    m_children.push_back(object);
}

vec3 Object3D::getPosition() {
    return m_position;
}

quat Object3D::getRotation() {
    return m_rotation;
}

vec3 Object3D::getScale() {
    return m_scale;
}

void Object3D::setPosition(const vec3& position) {
    m_position = position;
    m_worldPosMatrixNeedsUpdate = true;
}

void Object3D::setRotation(const quat& rotation) {
    m_rotation = glm::normalize(rotation); // @TODO check if required?
    m_worldPosMatrixNeedsUpdate = true;
}

void Object3D::setRotation(const vec3& euler) {

    m_rotation = quat(vec3(
        glm::radians(euler.x), 
        glm::radians(euler.y), 
        glm::radians(euler.z)
    ));
    m_rotation = glm::normalize(m_rotation);
    
    m_worldPosMatrixNeedsUpdate = true;

}

void Object3D::setScale(float uniformScale) {
    setScale(vec3(uniformScale));
}

void Object3D::setScale(const vec3& scale) {
    m_scale = scale;
    m_worldPosMatrixNeedsUpdate = true;
}

void Object3D::translate(vec3 translation) {
    m_position += translation;
    m_worldPosMatrixNeedsUpdate = true;
}

void Object3D::rotate(float angle, const vec3& direction) {
    m_rotation = glm::rotate(m_rotation, glm::radians(angle), direction);
    m_rotation = glm::normalize(m_rotation);
    m_worldPosMatrixNeedsUpdate = true;
}

mat4 Object3D::getWorldPositionMatrix() {
    if(m_worldPosMatrixNeedsUpdate) {

        m_worldPositionMatrix = glm::scale(
            glm::translate(
                glm::mat4(1),
                m_position
            ) * glm::mat4_cast(
                m_rotation
            ), 
            m_scale
        );

        if (m_parent)
            m_worldPositionMatrix = m_parent->m_worldPositionMatrix * m_worldPositionMatrix;

        for (auto&& child : m_children) {
            child->m_worldPosMatrixNeedsUpdate = true;
        }

        m_worldPosMatrixNeedsUpdate = false;
    }
    
    return m_worldPositionMatrix;
}

