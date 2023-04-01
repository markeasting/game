#pragma once

#include "common.h"
#include "geom/Geometry.h"
#include "geom/index.h"
#include "gfx/Mesh.h"
#include "physics/Pose.h"

enum ColliderType {
    Box, Plane, Sphere, ConvexMesh
};

struct Collider {
public:

    ColliderType m_type = Sphere;

    vec3 m_relativePos = vec3(0.0f);

    // @TODO Box3 / AABB

    Collider() = default;
    virtual ~Collider() = default;

    virtual void updateGlobalPose(const Pose& pose);

    virtual void setRelativePos(vec3 pos) {
        m_relativePos = pos;
    }

    /* GJK */
    virtual vec3 findFurthestPoint(const vec3& dir) {
        return vec3(0);
    }

};

struct PlaneCollider : public Collider {
    
    vec2 m_size = vec2(1.0f, 1.0f);
    vec3 m_normal = vec3(0.0f, 0.0f, 1.0f);
    vec3 m_normalRef = vec3(0.0f, 0.0f, 1.0f);

    PlaneCollider(const vec2 &size = { 1.0f, 1.0f }, const vec3 &normal = vec3(0.0f, 1.0f, 0.0f));
    void updateGlobalPose(const Pose& pose);
};

struct SphereCollider : public Collider {
    float m_radius = 1.0f;
    SphereCollider(const float &diameter = 1.0f);
};

struct MeshCollider : public Collider {

    std::vector<vec3> m_vertices;
    std::vector<vec3> m_verticesWorldSpace;
    std::vector<unsigned int> m_indices;
    std::vector<unsigned int> m_uniqueIndices;

    // @TODO convexHull;

    MeshCollider(Ref<Geometry> convexGeometry);
    MeshCollider(Ref<Mesh> mesh);
    
    void setGeometry(Ref<Geometry> geometry);
    void updateGlobalPose(const Pose& pose);

    vec3 findFurthestPoint(const vec3& dir);
    
};

struct BoxCollider : public MeshCollider {
    vec3 m_size = vec3(1.0f, 1.0f, 1.0f);
    BoxCollider(const vec3 &size = { 1.0f, 1.0f, 1.0f });
};
