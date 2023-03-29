#pragma once

#include "common.h"
#include "geom/Geometry.h"
#include "gfx/Mesh.h"

enum ColliderType {
    Box, Plane, Sphere, ConvexMesh
};

struct Collider {
public:

    ColliderType colliderType = Sphere;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> uniqueIndices;

    glm::vec3 relativePos = glm::vec3(0.0f);

    Collider() = default;
    ~Collider() = default;
    
    void setGeometry(Ref<Geometry> geometry);

    virtual void updateRotation(const glm::quat& rotation);

};

struct BoxCollider : public Collider {
    glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);
    BoxCollider(const glm::vec3 &size = { 1.0f, 1.0f, 1.0f });
};

struct PlaneCollider : public Collider {
    glm::vec2 size = glm::vec2(1.0f, 1.0f);
    glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 normalRef = glm::vec3(0.0f, 0.0f, 1.0f);
    PlaneCollider(const glm::vec2 &size = { 1.0f, 1.0f }, const glm::vec3 &normal = glm::vec3(0.0f, 1.0f, 0.0f));
    void updateRotation(const glm::quat& rotation);
};

struct SphereCollider : public Collider {
    float radius = 1.0f;
    SphereCollider(const float &diameter = 1.0f);
};

struct MeshCollider : public Collider {
    MeshCollider(Ref<Geometry> convexGeometry);
    MeshCollider(Ref<Mesh> mesh);
};
