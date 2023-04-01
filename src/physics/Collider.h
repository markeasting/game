#pragma once

#include "common.h"
#include "geom/Geometry.h"
#include "geom/index.h"
#include "gfx/Mesh.h"

enum ColliderType {
    Box, Plane, Sphere, ConvexMesh
};

struct Collider {
public:

    ColliderType m_type = Sphere;

    glm::vec3 m_relativePos = glm::vec3(0.0f);

    Collider() = default;
    ~Collider() = default;

    virtual void updateRotation(const glm::quat& rotation);

};

struct PlaneCollider : public Collider {
    
    glm::vec2 m_size = glm::vec2(1.0f, 1.0f);
    glm::vec3 m_normal = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 m_normalRef = glm::vec3(0.0f, 0.0f, 1.0f);

    PlaneCollider(const glm::vec2 &size = { 1.0f, 1.0f }, const glm::vec3 &normal = glm::vec3(0.0f, 1.0f, 0.0f));
    void updateRotation(const glm::quat& rotation);
};

struct SphereCollider : public Collider {
    float m_radius = 1.0f;
    SphereCollider(const float &diameter = 1.0f);
};

struct MeshCollider : public Collider {

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<unsigned int> m_uniqueIndices;

    MeshCollider(Ref<Geometry> convexGeometry);
    MeshCollider(Ref<Mesh> mesh);
    
    void setGeometry(Ref<Geometry> geometry);
};

struct BoxCollider : public MeshCollider {
    glm::vec3 m_size = glm::vec3(1.0f, 1.0f, 1.0f);
    BoxCollider(const glm::vec3 &size = { 1.0f, 1.0f, 1.0f });
};
