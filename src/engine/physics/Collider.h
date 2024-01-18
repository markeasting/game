#pragma once

#include "engine/geom/Geometry.h"
#include "engine/gfx/Mesh.h"
#include "engine/physics/Pose.h"
#include "engine/physics/AABB.h"
#include "engine/physics/Plane.h"

#include <vector>
#include <array>

enum class ColliderType {
    BOX, 
    PLANE, 
    SPHERE, 
    INEFFICIENT_MESH,
    CONVEX_MESH
};

struct Collider {
public:

    ColliderType m_type = ColliderType::SPHERE;

    vec3 m_relativePos = vec3(0.0f);
    vec3 m_relativePosW = vec3(0.0f);
    
    Ref<Mesh> m_mesh = nullptr; // Debug mesh

    AABB m_aabb;
    AABB m_expanded_aabb;

    // m_sdf; // @TODO distance function?

    Collider() = default;
    virtual ~Collider() = default;

    virtual void expandAABB(float scalar = 0.0f);
    virtual void updateGlobalPose(const Pose& pose);

    virtual void setRelativePos(const vec3& pos) {
        m_relativePos = pos;
    }

    /* GJK */
    virtual vec3 findFurthestPoint(const vec3& dir) const {
        return vec3(0);
    }

};

struct PlaneCollider : public Collider {
    
    Plane m_plane;

    PlaneCollider(const vec2 &size = { 1.0f, 1.0f }, const vec3 &normal = vec3(0.0f, 1.0f, 0.0f));
    void updateGlobalPose(const Pose& pose) override;
};

struct SphereCollider : public Collider {
    float m_radius = 1.0f;
    SphereCollider(const float diameter = 1.0f);
};

struct MeshCollider : public Collider {

    std::vector<vec3> m_vertices;
    std::vector<vec3> m_verticesWorldSpace;
    std::vector<unsigned int> m_indices;
    std::vector<unsigned int> m_uniqueIndices;

    std::vector<std::array<vec3, 4>> m_triangles;

    // @TODO convexHull;

    MeshCollider(Ref<Geometry> geometry, bool isConvex = true);
    MeshCollider(Ref<Mesh> mesh);
    
    void setGeometry(Ref<Geometry> geometry);
    void setRelativePos(const vec3& pos) override;
    void expandAABB(float scalar = 0.0f) override;
    void updateGlobalPose(const Pose& pose) override;

    vec3 findFurthestPoint(const vec3& dir) const override;
    
};

struct BoxCollider : public MeshCollider {
    vec3 m_size = vec3(1.0f, 1.0f, 1.0f);
    BoxCollider(float size = 1.0f);
    BoxCollider(const vec3 &size = { 1.0f, 1.0f, 1.0f });
};
