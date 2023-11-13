#pragma once

#include "physics/RigidBody.h"
#include "physics/Constraint.h"
#include "gfx/Mesh.h"

struct RaycastInfo {
    bool exists = false;
    vec3 point = vec3();
    vec3 normal = vec3();
    float dist = 0.0f;
};

class PhysicsHandler {
public:

	std::vector<Ref<RigidBody>> m_bodies = {};
	std::vector<Ref<Constraint>> m_constraints = {};
    std::vector<Ref<Mesh>> m_debugMeshes;

    PhysicsHandler() = default;
    ~PhysicsHandler() = default;

    void add(Ref<RigidBody> body);
    // void addStaticMesh(Ref<Mesh> mesh);

    void init();
    
    void update(float dt, std::function<void(float)> customUpdate);

    RaycastInfo raycast(const vec3& ray_origin, const vec3& ray_dir);

private:
	// std::vector<Ref<Mesh>> m_staticMeshes = {};

};
