#pragma once

#include "common.h"
#include "physics/RigidBody.h"
#include "physics/PhysicsHandler.h"
// #include "physics/ContactSet.h"

struct CollisionPair {
    RigidBody* A = NULL;
    RigidBody* B = NULL;
};

struct ContactSet {
    RigidBody* A = NULL;
    RigidBody* B = NULL;
    glm::vec3 p1 = glm::vec3(0.0f);
    glm::vec3 p2 = glm::vec3(0.0f);
    glm::vec3 r1 = glm::vec3(0.0f);
    glm::vec3 r2 = glm::vec3(0.0f);

    glm::vec3 n = glm::vec3(0.0f);
    float d = 0.0f;
    glm::vec3 vrel = glm::vec3(0.0f);
    float vn = 0.0f;

    float e = 0.5f;
    float staticFriction = 0.0f;
    float dynamicFriction = 0.0f;
    float lambdaN = 0.0f;
    float lambdaT = 0.0f;

    ContactSet(RigidBody* A, RigidBody* B): A(A), B(B) {}
};

namespace XPBDSolver {

    const int numSubSteps = 20;

    void update(const std::vector<Ref<RigidBody>>& rigidBodies, const float& dt);

    std::vector<CollisionPair> collectCollisionPairs(const std::vector<Ref<RigidBody>>& rigidBodies, const float& dt);
    std::vector<ContactSet*> getContacts(const std::vector<CollisionPair>& collisions);

    void solvePositions(const std::vector<ContactSet*>& contacts, const float& h);
    void solveVelocities(const std::vector<ContactSet*>& contacts, const float& h);

    void _solvePenetration(ContactSet* contact, const float& h);
    void _solveFriction(ContactSet* contact, const float& h);

    float applyBodyPairCorrection(
        RigidBody* body0,
        RigidBody* body1,
        // const ContactSet* contact,
        const glm::vec3& corr,
        const float& compliance,
        const float& dt,
        const glm::vec3& pos0 = glm::vec3(0.0f),
        const glm::vec3& pos1 = glm::vec3(0.0f),
        const bool& velocityLevel = false
    );

}
