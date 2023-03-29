#pragma once

#include "physics/XPBDSolver.h"
#include "physics/RigidBody.h"
#include "physics/Collider.h"

enum SolverType {
    XPBD
};

class PhysicsHandler {
public:

    const SolverType solverType = SolverType::XPBD;

	std::vector<Ref<RigidBody>> bodies = {};

	// static PhysicsHandler& Instance();
	// PhysicsHandler(const PhysicsHandler&) = delete;
    PhysicsHandler() = default;
    ~PhysicsHandler() = default;

    void Enqueue(Ref<RigidBody> body);

    void update(float dt);

private:
    // PhysicsHandler();

    unsigned int bodyID = 0;

};
