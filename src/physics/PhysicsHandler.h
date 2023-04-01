#pragma once

#include "physics/RigidBody.h"

class PhysicsHandler {
public:

	std::vector<Ref<RigidBody>> m_bodies = {};

    PhysicsHandler() = default;
    ~PhysicsHandler() = default;

    void Enqueue(Ref<RigidBody> body);

    void update(float dt);

private:

};
