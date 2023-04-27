
#include "physics/PhysicsHandler.h"
#include "physics/XPBDSolver.h"

void PhysicsHandler::init() {
    XPBDSolver::init();

    m_debugMeshes.push_back(XPBDSolver::p1);
    m_debugMeshes.push_back(XPBDSolver::p2);
    m_debugMeshes.push_back(XPBDSolver::r1);
    m_debugMeshes.push_back(XPBDSolver::r2);
    m_debugMeshes.push_back(XPBDSolver::n);
    m_debugMeshes.push_back(XPBDSolver::debugArrow);
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
void PhysicsHandler::update(float dt, std::function<void(float)> customUpdate) {

    XPBDSolver::update(m_bodies, m_constraints, customUpdate, dt);

}

std::tuple<vec3, vec3, float, Ref<RigidBody>> PhysicsHandler::raycast(const vec3& ray_origin, const vec3& ray_dir) {

    vec3 hitPoint;
    vec3 hitNormal;
    float hitDistance = FLT_MAX;
    Ref<RigidBody> hitBody = nullptr;

    for (const auto& body: m_bodies) {

        if (body->collider->m_type != cPlane)
            continue;
    
        /* @TODO check if plane is properly transformed */
        const auto& PC = static_cast<PlaneCollider*>(body->collider.get());
        const vec3 normal = PC->m_plane.normal;
        const float C = PC->m_plane.constant;

        float d = -glm::dot(ray_dir, normal);
        
        // Skip if ray is parallel or ray_origining away from the plane
        if (d < 0.001f)
            continue;
            
        float distance = glm::dot(ray_origin - body->pose.p, normal) / d;

        if (distance < hitDistance) {
            hitPoint = ray_origin + ray_dir * distance;
            hitDistance = distance;
            hitNormal = normal;
            hitBody = body;
        }

    }

    return std::make_tuple(
        hitPoint,
        hitNormal,
        hitDistance,
        hitBody
    );

}
