#pragma once

#include "physics/RigidBody.h"
#include "gfx/Mesh.h"

class PhysicsHandler {
public:

	std::vector<Ref<RigidBody>> m_bodies = {};

    PhysicsHandler() = default;
    ~PhysicsHandler() = default;

    void Enqueue(Ref<RigidBody> body);

    void init();
    
    void update(float dt);

    std::vector<Ref<Mesh>> m_debugMeshes;

private:

};
