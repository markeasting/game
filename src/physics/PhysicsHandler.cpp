
#include "physics/PhysicsHandler.h"
#include "physics/XPBDSolver.h"

void PhysicsHandler::init() {
    XPBDSolver::init();

    m_debugMeshes.push_back(XPBDSolver::p1);
    m_debugMeshes.push_back(XPBDSolver::p2);
    m_debugMeshes.push_back(XPBDSolver::r1);
    m_debugMeshes.push_back(XPBDSolver::r2);
    m_debugMeshes.push_back(XPBDSolver::n);
}

void PhysicsHandler::add(Ref<RigidBody> body) {
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

    XPBDSolver::update(m_bodies, m_constraints, dt);

}

float PhysicsHandler::raycast(const vec3& point, const vec3& ray_direction) {

    float minDist = FLT_MAX;

    for (const auto& body: m_bodies) {

        if (body->collider->m_type != cPlane)
            continue;
    
        /* @TODO check if plane is properly transformed */
        const auto& PC = static_cast<PlaneCollider*>(body->collider.get());
        const vec3 normal = PC->m_plane.normal;
        const float C = PC->m_plane.constant;

        float d = -glm::dot(ray_direction, normal);
        
        // Skip if ray is parallel or pointing away from the plane
        if (d < 0.001f)
            continue;
            
        float distance = glm::dot(point - body->pose.p, normal) / d;

        minDist = std::min(minDist, distance);
    }

    return minDist;

}
