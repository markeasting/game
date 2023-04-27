#pragma once

#include "common.h"
#include "physics/RigidBody.h"
#include "physics/gjk-epa/GjkEpa.h"
// #include "physics/ContactSet.h"
#include "physics/Constraint.h"

#include "gfx/Mesh.h"

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

    ContactSet(
        RigidBody* A, 
        RigidBody* B,
        vec3 normal,
        float d,
        vec3 _p1,
        vec3 _p2,
        vec3 _r1,
        vec3 _r2
    ): A(A), B(B), n(normal), d(d), p1(_p1), p2(_p2), r1(_r1), r2(_r2) {

        assert(A != B);

        /*
         * (29) Relative velocity
         * This is calculated before the velocity solver,
         * so that we can solve restitution (Eq. 34).
         * 
         * ṽn == vn
         */
        vrel = A->getVelocityAt(p1) - B->getVelocityAt(p2);
        vn = glm::dot(n, vrel);

        e = 0.5f * (A->bounciness + B->bounciness);
        staticFriction = 0.5f * (A->staticFriction + B->staticFriction);
        dynamicFriction = 0.5f * (A->dynamicFriction + B->dynamicFriction);

        /* (3.5) Penetration depth -- Note: sign was flipped! 
         * 
         * Note: d is often already calculated when finding collisions, so
         *       it's already given as an argument. This is often (EPS) equal
         *       to the equation from (3.5).
         */
        // d = - glm::dot((p1 - p2), n);

    }

    inline void update() {
        p1 = A->pose.p + A->pose.q * r1;
        p2 = B->pose.p + B->pose.q * r2;

        // n = glm::normalize(p2 - p1); // BREAKS

        // if (glm::distance(p2, p1) > 0.001f) {
        //     n = glm::normalize(p2 - p1);
        // }

        /* (3.5) Penetration depth */
        d = - glm::dot((p1 - p2), n);
    }
};

namespace XPBDSolver {

    const int numSubSteps = 15;

    inline Ref<Mesh> p1;
    inline Ref<Mesh> p2;
    inline Ref<Mesh> r1;
    inline Ref<Mesh> r2;
    inline Ref<Mesh> n;

    inline Ref<Mesh> debugArrow;

    void init();

    void update(const std::vector<Ref<RigidBody>>& bodies, const std::vector<Ref<Constraint>>& constraints, std::function<void(float)> extraUpdate, const float dt);

    std::vector<CollisionPair> collectCollisionPairs(const std::vector<Ref<RigidBody>>& rigidBodies, const float dt);
    std::vector<Ref<ContactSet>> getContacts(const std::vector<CollisionPair>& collisions);

    void solvePositions(const std::vector<Ref<ContactSet>>& contacts, const float h);
    void solveVelocities(const std::vector<Ref<ContactSet>>& contacts, const float h);

    void _solvePenetration(Ref<ContactSet> contact, const float h);
    void _solveFriction(Ref<ContactSet> contact, const float h);

    float applyBodyPairCorrection(
        RigidBody* body0,
        RigidBody* body1,
        const glm::vec3& corr,
        const float compliance,
        const float dt,
        const glm::vec3& pos0 = glm::vec3(0.0f),
        const glm::vec3& pos1 = glm::vec3(0.0f),
        const bool velocityLevel = false
    );

    void debugContact(Ref<ContactSet> contact);

}
