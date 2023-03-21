#pragma once

#include "common.h"

class Object3D {
public:

    Object3D() = default;
    virtual ~Object3D() {};
    
    // const char* name = "";
    // const char* type = "Object3D"; // @TODO make enum or use instanceof?

    bool m_useProjectionMatrix = true;

    // std::weak_ptr<Object3D> parent;
    Object3D* parent = nullptr; // @TODO use weak_ptr?
    std::vector<Ref<Object3D>> children = {};

    // @TODO convert these to chaining pattern
    // https://levelup.gitconnected.com/how-to-implement-method-chaining-in-c-3ec9f255972a
    void add(Ref<Object3D> object);
    
    void setPosition(const vec3& position);

    void setRotation(const vec3& euler);
    void setRotation(const quat& rotation);
    
    void setScale(float uniformScale);
    void setScale(const vec3& scale);
    
    void translate(vec3 translation);
    void rotate(float angle, const vec3& direction);

    vec3 getPosition();
    quat getRotation();
    vec3 getScale();

    mat4 getWorldPositionMatrix();

protected:

    vec3 m_position = vec3(0.0f, 0.0f, 0.0f);
    quat m_rotation = quat(1.0f, 0, 0, 0);
    vec3 m_scale = vec3(1.0f, 1.0f, 1.0f);

private:

    bool m_worldPosMatrixNeedsUpdate = true;
    mat4 m_worldPositionMatrix = mat4(1.0f);

};

// using Object3DRef = Ref<Object3D>;
