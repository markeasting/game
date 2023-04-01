
#include "physics/PhysicsHandler.h"
#include "physics/XPBDSolver.h"

void PhysicsHandler::Enqueue(Ref<RigidBody> body) {
    assert(body != nullptr);

    body->id = m_bodies.size();
    m_bodies.push_back(body);
}

// void PhysicsHandler::Enqueue(Mesh* mesh) {
//     Enqueue(new RigidBody(mesh));
// }

// This entire loop should probably be running on a separate thread.
// https://medium.com/@cancerian0684/singleton-design-pattern-and-how-to-make-it-thread-safe-b207c0e7e368
void PhysicsHandler::update(float dt) {

    XPBDSolver::update(m_bodies, dt);

}
