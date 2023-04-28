
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

// void PhysicsHandler::addStaticMesh(Ref<Mesh> mesh) {
//     assert(mesh != nullptr);

//     m_staticMeshes.push_back(mesh);
// }

// This entire loop should probably be running on a separate thread.
// https://medium.com/@cancerian0684/singleton-design-pattern-and-how-to-make-it-thread-safe-b207c0e7e368
void PhysicsHandler::update(float dt, std::function<void(float)> customUpdate) {

    XPBDSolver::update(m_bodies, m_constraints, customUpdate, dt);

}

static std::tuple<bool, vec3, float, vec3> _raycastPlane(const Plane& plane, const vec3& ray_origin, const vec3& ray_dir, bool usePlaneSize = true) {
    
    float d = -glm::dot(ray_dir, plane.normal);
    
    // Skip if ray is parallel or pointing away from the plane
    if (d < 0.001f)
        return std::make_tuple(
            false,
            vec3(),
            0.0f,
            vec3()
        );
    
    // @TODO get rid of plane.origin at some point?
    float distance = glm::dot(ray_origin - plane.origin, plane.normal) / d;
    vec3 hit = ray_origin + ray_dir * distance;

    return std::make_tuple(
        usePlaneSize ? plane.containsPoint(hit) : true,
        hit,
        distance,
        plane.normal
    );
}

// bool isPointInsidePolygon(const vec3& point, const vec3& p0, const vec3& p1, const vec3& p2, vec3 n = vec3(0)) {
//     // Step 1: Compute the normal vector
//     glm::vec3 u = p1 - p0;
//     glm::vec3 v = p2 - p0;
//     // glm::vec3 n = glm::cross(u, v);

//     // Step 2: Check if the point is on the same side of the plane as the triangle
//     glm::vec3 w = point - p0;
//     if (glm::dot(n, w) < 0) {
//         return false;
//     }

//     // Step 3: Compute barycentric coordinates
//     float uu = glm::dot(u, u);
//     float uv = glm::dot(u, v);
//     float vv = glm::dot(v, v);
//     glm::vec3 wu = point - p0;
//     float wuv = glm::dot(wu, v);
//     float wuu = glm::dot(wu, u);
//     float den = uv * uv - uu * vv;

//     // Step 4: Compute s and t
//     float s = (uv * wuv - vv * wuu) / den;
//     float t = (uv * wuu - uu * wuv) / den;

//     // Step 5: Check if the point is within the triangle
//     return s >= 0 && t >= 0 && s + t <= 1;
// }

std::tuple<vec3, vec3, float, Ref<RigidBody>> PhysicsHandler::raycast(const vec3& ray_origin, const vec3& ray_dir) {

    vec3 hitPoint;
    vec3 hitNormal;
    float hitDistance = FLT_MAX;
    Ref<RigidBody> hitBody = nullptr;

    for (const auto& body: m_bodies) {

        if (body->collider->m_type != cPlane)
            continue;
    
        const auto& PC = static_cast<PlaneCollider*>(body->collider.get());

        auto [exists, hit, dist, normal] = _raycastPlane(PC->m_plane, ray_origin, ray_dir);

        if (exists && dist < hitDistance) {
            hitPoint = hit;
            hitDistance = dist;
            hitNormal = normal;
            hitBody = body;
        }

    }

    // for (const auto& mesh : m_staticMeshes) {
    //     const auto& vertices = mesh->m_geometry->m_vertexBuffer->m_data;
    //     const auto& indices = mesh->m_geometry->m_indexBuffer->m_data;

    //     // const auto& matrix = mesh->getWorldPositionMatrix();

    //     int it = 0;
    //     for (size_t i = 0; i < indices.size(); i += 3) {
    //         // @TODO precompute since the meshes are static anyways...
    //         // @TODO use matrix instead of this mess
    //         const vec3 v1 = mesh->getPosition() + mesh->getRotation() * vertices[indices[i]].position;
    //         const vec3 v2 = mesh->getPosition() + mesh->getRotation() * vertices[indices[i + 1]].position;
    //         const vec3 v3 = mesh->getPosition() + mesh->getRotation() * vertices[indices[i + 2]].position;

    //         Plane plane;
    //         plane.setFromCoplanarPoints(v1, v2, v3);

    //         auto [exists, hit, dist, normal] = _raycastPlane(plane, ray_origin, ray_dir, false);

    //         if (!exists || !isPointInsidePolygon(hit, v1, v2, v3, normal))
    //             continue;

    //         if (dist < hitDistance) {
    //             hitPoint = hit;
    //             hitDistance = dist;
    //             hitNormal = normal;
    //             hitBody = nullptr;
    //         }
    //     }
    // }

    return std::make_tuple(
        hitPoint,
        hitNormal,
        hitDistance,
        hitBody
    );

}
