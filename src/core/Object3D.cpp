
#include "core/Object3D.h"

void Object3D::add(Ref<Object3D> object) {

    // @TODO error if object == this

    if (object->isObject3D) {
        if (object->parent) {
            // @TODO remove from parent
        }

        // @TODO figure out which style looks the cleanest

        // object->parent = std::make_shared<Object3D>(this);
        // object->children.push_back(std::make_shared<Object3D>(object));

        // object->parent = Object3DRef(this);
        // object->children.push_back(Object3DRef(object));

        // object->parent = Ref<Object3D>(this);
        // object->children.push_back(Ref<Object3D>(object));
    }
}

glm::vec3 Object3D::getPosition() {
    return this->position;
}

glm::quat Object3D::getRotation() {
    return this->rotation;
}

glm::vec3 Object3D::getScale() {
    return this->scale;
}

void Object3D::setPosition(glm::vec3 position) {
    // if(!managedByRigidBody) {
        this->worldPosMatrixNeedsUpdate = true;
        this->position = position;
    // }
}

void Object3D::setRotation(const glm::quat& rotation) {
    // if(!managedByRigidBody) {
        this->worldPosMatrixNeedsUpdate = true;
        this->rotation = glm::normalize(rotation);
    // }
}

void Object3D::setRotation(const glm::vec3& eulerRotation) {
    // if(!managedByRigidBody) {
        this->worldPosMatrixNeedsUpdate = true;

        // There must be a faster way
        this->rotation = glm::rotate(this->rotation, glm::radians(eulerRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        this->rotation = glm::rotate(this->rotation, glm::radians(eulerRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        this->rotation = glm::rotate(this->rotation, glm::radians(eulerRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        
        this->rotation = glm::normalize(this->rotation);
    // }
}

void Object3D::setScale(const float& uniformScale) {
    this->setScale(glm::vec3(uniformScale));
}

void Object3D::setScale(const glm::vec3& scale) {
    this->scale = scale;
}

void Object3D::translate(glm::vec3 translation) {
    // if(!managedByRigidBody) {
        this->worldPosMatrixNeedsUpdate = true;
        this->position += translation;
    // }
}

void Object3D::rotate(float angle, glm::vec3 direction) {
    // if(!managedByRigidBody) {
        this->worldPosMatrixNeedsUpdate = true;
        this->rotation = glm::rotate(this->rotation, glm::radians(angle), direction);
        this->rotation = glm::normalize(this->rotation);
    // }
}

glm::mat4 Object3D::getWorldPositionMatrix() {
    if(this->worldPosMatrixNeedsUpdate) {
        this->worldPositionMatrix = glm::scale(glm::translate(glm::mat4(1), this->position) * glm::mat4_cast(this->rotation), this->scale);
        this->worldPosMatrixNeedsUpdate = false;
    }
    
    return this->worldPositionMatrix;
}

