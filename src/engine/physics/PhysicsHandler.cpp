
#include "engine/physics/PhysicsHandler.h"
#include "engine/physics/XPBDSolver.h"
#include <glm/gtx/intersect.hpp>

#include <array>

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

// This entire loop should probably be running on a separate thread.
// https://medium.com/@cancerian0684/singleton-design-pattern-and-how-to-make-it-thread-safe-b207c0e7e368
void PhysicsHandler::update(float dt, std::function<void(float)> customUpdate) {

    XPBDSolver::update(m_bodies, m_constraints, customUpdate, dt);

}

// static std::tuple<bool, vec3, float, vec3> _raycastPlane(const Plane& plane, const vec3& ray_origin, const vec3& ray_dir, bool usePlaneSize = true) {
    
//     float d = -glm::dot(ray_dir, plane.normal);
    
//     // Skip if ray is parallel or pointing away from the plane
//     if (d < 0.001f)
//         return std::make_tuple(
//             false,
//             vec3(),
//             0.0f,
//             vec3()
//         );
    
//     // @TODO get rid of plane.origin at some point?
//     float distance = glm::dot(ray_origin - plane.origin, plane.normal) / d;
//     vec3 hit = ray_origin + ray_dir * distance;

//     return std::make_tuple(
//         usePlaneSize ? plane.containsPoint(hit) : true,
//         hit,
//         distance,
//         plane.normal
//     );
// }

/** Moller–Trumbore algorithm */
static bool rayTriangleIntersect(
    const vec3 &ro,
    const vec3 &rd,
    const std::array<vec3, 4> &triangle,
    float &d
) { 

    vec2 bary;

    return glm::intersectRayTriangle(ro, rd, triangle[0], triangle[1], triangle[2], bary, d);

    // const float EPS = 0.000001f;

    // const vec3& vertex0 = triangle[0];
    // const vec3& vertex1 = triangle[1];
    // const vec3& vertex2 = triangle[2];

    // vec3 edge1, edge2, h, s, q;
    // float a, f, u, v;

    // edge1 = vertex1 - vertex0;
    // edge2 = vertex2 - vertex0;
    // h = glm::cross(rd, edge2);
    // a = glm::dot(edge1, h);

    // if (a > -EPS && a < EPS)
    //     return false;

    // f = 1.0f / a;
    // s = ro - vertex0;
    // u = f * glm::dot(s, h);

    // if (u < 0.0f || u > 1.0f)
    //     return false;

    // q = glm::cross(s, edge1);
    // v = f * glm::dot(rd, q);

    // if (v < 0.0f || u + v > 1.0f)
    //     return false;

    // d = f * glm::dot(edge2, q);

    // if (d > EPS)
    //     return true;

    // return false;
}

RaycastInfo PhysicsHandler::raycast(const vec3& ray_origin, const vec3& ray_dir) {

    RaycastInfo result;
    float d;
    float minDistance = FLT_MAX;
    std::array<vec3, 4> tempTriangle;

    for (const auto& body: m_bodies) {

        /* Quick hack, need some kind of masking / filtering system */ 
        if (body->name == "CarBody")
            continue;

        if (body->collider->m_type == ColliderType::CONVEX_MESH || body->collider->m_type == ColliderType::INEFFICIENT_MESH) {
            const auto& MC = std::static_pointer_cast<MeshCollider>(body->collider);
            
            for (const auto& triangle : MC->m_triangles) {

                if (rayTriangleIntersect(ray_origin, ray_dir, triangle, d)) {
                    if (d > 0.0f && d < minDistance) {
                        minDistance = d;
                        result.exists = true;
                        result.dist = d;
                        result.point = ray_origin + d * ray_dir;

                        // result.normal = triangle[3];
                        tempTriangle = triangle;
                    }
                };
            }
        }

        if (result.exists) {
            /* Calculate normal */
            vec3 edge1 = tempTriangle[1] - tempTriangle[0];
            vec3 edge2 = tempTriangle[2] - tempTriangle[0];
            result.normal = glm::normalize(glm::cross(edge1, edge2));
            // result.normal = tempTriangle[3];
        }
    }

    return result;

}
