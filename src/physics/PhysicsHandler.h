#pragma once

#include "physics/RigidBody.h"
#include "physics/Constraint.h"
#include "gfx/Mesh.h"

class PhysicsHandler {
public:

	std::vector<Ref<RigidBody>> m_bodies = {};
	std::vector<Ref<Constraint>> m_constraints = {};
    std::vector<Ref<Mesh>> m_debugMeshes;

    PhysicsHandler() = default;
    ~PhysicsHandler() = default;

    void add(Ref<RigidBody> body);

    void init();
    
    void update(float dt);

    float raycast(const vec3& origin, const vec3& dir);

private:

};
