
#include "physics/PhysicsHandler.h"

// PhysicsHandler& PhysicsHandler::Instance() {
//     static PhysicsHandler instance;
//     return instance;
// }

// PhysicsHandler::PhysicsHandler() {}

void PhysicsHandler::Enqueue(Ref<RigidBody> body) {
    assert(body != nullptr);

    body->id = this->bodyID++;
    this->bodies.push_back(body);
}

// void PhysicsHandler::Enqueue(Mesh* mesh) {
//     this->Enqueue(new RigidBody(mesh));
// }

// This entire loop should probably be running on a separate thread.
// https://medium.com/@cancerian0684/singleton-design-pattern-and-how-to-make-it-thread-safe-b207c0e7e368
void PhysicsHandler::update(float dt) {

    switch(PhysicsHandler::solverType) {

        case SolverType::XPBD :
            XPBDSolver::update(this->bodies, dt);
        default :

        break;
    }

}
