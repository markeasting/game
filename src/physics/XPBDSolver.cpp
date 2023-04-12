#include "physics/XPBDSolver.h"

void XPBDSolver::init() {
    Material colorMaterial = Material("Basic");
    colorMaterial.wireframe = true;

    XPBDSolver::p1 = ref<Mesh>(BoxGeometry(0.05f), colorMaterial);
    XPBDSolver::p2 = ref<Mesh>(BoxGeometry(0.05f), colorMaterial);
    XPBDSolver::r1 = ref<Mesh>(ArrowGeometry(1.0f), colorMaterial);
    XPBDSolver::r2 = ref<Mesh>(ArrowGeometry(1.0f), colorMaterial);
    XPBDSolver::n = ref<Mesh>(ArrowGeometry(1.0f), colorMaterial);
}


glm::quat QuatFromTwoVectors(glm::vec3 vFrom, glm::vec3 vTo) {
    // https://github.com/Mugen87/three.js/blob/master/src/math/Quaternion.js

    vFrom = glm::normalize(vFrom);
    vTo = glm::normalize(vTo);
    
    float r;
    float EPS = 0.000001;
    glm::vec3 v1 = glm::vec3();

    r = glm::dot(vFrom, vTo ) + 1.0f;

    if ( r < EPS ) {
        r = 0;
        if ( abs( vFrom.x ) > abs( vFrom.z ) ) {
            v1 = { -vFrom.y, vFrom.x, 0 };
        } else {
            v1 = { 0, - vFrom.z, vFrom.y };
        }
    } else {
        v1 = glm::cross( vFrom, vTo );
    }

    glm::quat result;

    result.x = v1.x;
    result.y = v1.y;
    result.z = v1.z;
    result.w = r;

    return glm::normalize(result);    
}


void XPBDSolver::update(const std::vector<Ref<RigidBody>>& bodies, const float& dt) {

    /* XPBD algorithm 2 */

    /* (3.5)
     * To save computational cost we collect potential
     * collision pairs once per time step instead of once per
     * sub-step using a tree of axis aligned bounding boxes.
     */
    auto collisions = XPBDSolver::collectCollisionPairs(bodies, dt);

    // const double h = dt / XPBDSolver::numSubSteps;
    const double h = (1.0f / 60.0f) / XPBDSolver::numSubSteps;

    for(int i = 0; i < XPBDSolver::numSubSteps; i++) {

        /* (3.5)
         * At each substep we iterate through the pairs
         * checking for actual collisions.
         */
        auto contacts = XPBDSolver::getContacts(collisions);

        for (auto const& body: bodies) {
            body->integrate(h);
        }

        XPBDSolver::solvePositions(contacts, h);

        for (auto const& body: bodies) {
            body->update(h);
        }

        XPBDSolver::solveVelocities(contacts, h);

    }

    // @TODO move to start of body->integrate?
    for (auto const& body: bodies) {
        body->force = vec3(0.0f);
        body->torque = vec3(0.0f);
        body->updateGeometry();
    }
}

std::vector<CollisionPair> XPBDSolver::collectCollisionPairs(const std::vector<Ref<RigidBody>>& rigidBodies, const float& dt) {

    // @TODO Chunking / octree
    // https://github.com/mwarning/SimpleOctree/tree/master/src

    std::vector<CollisionPair> collisions = {};

    std::vector<std::string> pairs = {};

    for (auto const& A: rigidBodies) {
        for (auto const& B: rigidBodies) {

            if ((A == B) || (!A->isDynamic && !B->isDynamic))
                continue;

            std::string pair = (A->id > B->id)
                ? std::to_string(A->id).append(std::to_string(B->id))
                : std::to_string(B->id).append(std::to_string(A->id));

            if(std::find(pairs.begin(), pairs.end(), pair) != pairs.end()) {
                continue; // Pair was already checked
            }

            pairs.push_back(pair);

            /* (3.5) k * dt * vbody */
            const float collisionMargin = 2.0f * dt * glm::length(A->vel - B->vel);

            AABB aabb1(A->collider->m_aabb);
            AABB aabb2(B->collider->m_aabb);
            aabb1.expandByScalar(collisionMargin);
            aabb2.expandByScalar(collisionMargin);

            switch(A->collider->m_type) {
                case ColliderType::cMesh :
                    switch(B->collider->m_type) {
                        case ColliderType::cMesh : {

                            if (aabb1.intersects(aabb2))
                                collisions.push_back({ A.get(), B.get() });

                            break;
                        }
                        case ColliderType::cPlane : {

                            // @TODO use a different / better cast?
                            const auto& MC = static_cast<MeshCollider*>(A->collider.get());
                            const auto& PC = static_cast<PlaneCollider*>(B->collider.get());

                            if (aabb1.intersectsPlane(PC->m_plane))
                                collisions.push_back({ A.get(), B.get() });

                            break;
                        }
                        default: break;
                    }
                break;
            }
        }
    }

    return collisions;
}

std::vector<ContactSet*> XPBDSolver::getContacts(const std::vector<CollisionPair>& collisions) {

    std::vector<ContactSet*> contacts = {};

    for (auto const& collision: collisions) {

        RigidBody* A = collision.A;
        RigidBody* B = collision.B;

        switch(A->collider->m_type) {
            case ColliderType::cMesh :
                switch(B->collider->m_type) {
                    case ColliderType::cMesh : {

                        Simplex simplex = GjkEpa::GJK(A->collider.get(), B->collider.get());

                        if (!simplex.containsOrigin)
                            break;

                        auto epa = GjkEpa::EPA(simplex, A->collider.get(), B->collider.get());
                        
                        if (!epa.exists || epa.d <= 0.0)
                            break;

                        auto contact = new ContactSet(
                            A,
                            B,
                            -epa.normal,
                            epa.p1,
                            epa.p2,
                            A->worldToLocal(epa.p1),
                            B->worldToLocal(epa.p1)
                        );

                        contacts.push_back(contact);
                        // XPBDSolver::debugContact(contact);

                        break;
                    }

                    case ColliderType::cPlane : {

                        const auto& MC = static_cast<MeshCollider*>(A->collider.get());
                        const auto& PC = static_cast<PlaneCollider*>(B->collider.get());

                        // @TODO check if vertex is actually inside plane size :)
                        // @TODO incorporate plane constant (dist from origin)
                        const vec3 N = PC->m_plane.normal;

                        for(int i = 0; i < MC->m_uniqueIndices.size(); i++) {

                            /* (26) - p1 */
                            const vec3 r1 = MC->m_vertices[MC->m_uniqueIndices[i]];
                            const vec3 p1 = MC->m_verticesWorldSpace[MC->m_uniqueIndices[i]];

                            /* (26) - p2 */
                            const float signedDistance = glm::dot(N, p1 - B->pose.p);
                            const vec3 p2 = p1 - (N * signedDistance);
                            const vec3 r2 = B->worldToLocal(p2);

                            /* (3.5) Penetration depth -- Note: sign was flipped! */
                            // const float d = - glm::dot((p1 - p2), N);
                            const float d = -signedDistance; // This matches the calculation above!

                            /* (3.5) if d ≤ 0 we skip the contact */
                            if (d <= 0.0f)
                                continue;

                            auto contact = new ContactSet(A, B, N, p1, p2, r1, r2);

                            contacts.push_back(contact);
                            // XPBDSolver::debugContact(contact);
                        }

                        break;
                    }
                    default: break;
                }
            break;
        }
    }

    return contacts;
}

void XPBDSolver::solvePositions(const std::vector<ContactSet*>& contacts, const float& h) {

    for (auto const& contact: contacts) {
        /* (3.5) Handling contacts and friction */
        XPBDSolver::_solvePenetration(contact, h);
        XPBDSolver::_solveFriction(contact, h);
    }
}

void XPBDSolver::_solvePenetration(ContactSet* contact, const float& h) {

    /* (26) - p1 & p2 */
    contact->update();

    /* (3.5) Penetration depth -- Note: sign was flipped! */
    contact->d = - glm::dot((contact->p1 - contact->p2), contact->n);

    /* (3.5) if d ≤ 0 we skip the contact */
    if(contact->d <= 0.0f)
        return;

    /* (3.5) Resolve penetration (Δx = dn using a = 0 and λn) */
    const vec3 dx = contact->d * contact->n;

    // XPBDSolver::debugContact(contact);

    float dlambda = XPBDSolver::applyBodyPairCorrection(
        contact->A,
        contact->B,
        dx,
        0.0f,
        h,
        contact->p1,
        contact->p2,
        false
    );

    /* (5) Update Lagrange multiplier */
    contact->lambdaN += dlambda;
}

void XPBDSolver::_solveFriction(ContactSet* contact, const float& h) {

    /* (3.5)
     * To handle static friction we compute the relative
     * motion of the contact points and its tangential component
     */

    /* (26) Positions in current state and before the substep integration */
    const vec3 p1prev = contact->p1; // A->prevPose.p + A->prevPose.q * contact->r1;
    const vec3 p2prev = contact->p2; // B->prevPose.p + B->prevPose.q * contact->r2;
    contact->update();

    /* (27) (28) Relative motion and tangential component */
    const vec3 dp = (contact->p1 - p1prev) - (contact->p2 - p2prev);
    const vec3 dp_t = dp - glm::dot(dp, contact->n) * contact->n;

    /* (3.5)
     * To enforce static friction, we apply Δx = Δp_t
     * at the contact points with a = 0 but only if
     * λ_t < μ_s * λ_n.
     *
     * Note: with 1 position iteration, lambdaT is always zero!
     */
    if (contact->lambdaT < contact->staticFriction * contact->lambdaN) {
        XPBDSolver::applyBodyPairCorrection(
            contact->A,
            contact->B,
            dp_t,
            0.0f,
            h,
            contact->p1,
            contact->p2,
            false
        );
    }
}

void XPBDSolver::solveVelocities(const std::vector<ContactSet*>& contacts, const float& h) {

    /* (3.6) Velocity level */

    for (auto const& contact: contacts) {

        contact->update();

        vec3 dv = vec3(0.0f);

        /* (29) Relative normal and tangential velocities
         *
         * Recalculate v and vn since the velocities are
         * solved *after* the body update step.
         */
        vec3 v = (
            contact->A->getVelocityAt(contact->p1) - 
            contact->B->getVelocityAt(contact->p2)
        );
        float vn = glm::dot(contact->n, v);
        vec3 vt = v - (contact->n * vn);

        /* (30) Friction */
        const float Fn = -contact->lambdaN / (h * h);
        const float friction = std::min(h * contact->dynamicFriction * Fn, glm::length(vt));
        dv -= glm::normalize(vt) * friction;

        /* (31, 32) @TODO dampening */

        /* (34) Restitution
         *
         * To avoid jittering we set e = 0 if vn is small (`threshold`).
         *
         * Note:
         * `vn_tilde` was already calculated before the position solve (Eq. 29)
         */
        const float threshold = (2.0f * 9.81f * h);
        const float e = std::abs(vn) <= threshold ? 0.0f : contact->e;
        const float vn_tilde = contact->vn;
        dv += contact->n * (-vn + std::max(-e * vn_tilde, 0.0f));

        /* (33) Velocity update */
        XPBDSolver::applyBodyPairCorrection(
            contact->A,
            contact->B,
            dv,
            0.0f,
            h,
            contact->p1,
            contact->p2,
            true
        );
    }
}

float XPBDSolver::applyBodyPairCorrection(
    RigidBody* body0,
    RigidBody* body1,
    const vec3& corr,
    const float& compliance,
    const float& dt,
    const vec3& pos0,
    const vec3& pos1,
    const bool& velocityLevel
) {

    const float C = glm::length(corr);

    if ( C < 1e-7 )
        return 0.0f;

    vec3 n = glm::normalize(corr);

    float w0 = body0->isDynamic ? body0->getInverseMass(n, pos0) : 0.0f;
    float w1 = body1->isDynamic ? body1->getInverseMass(n, pos1) : 0.0f;

    float w = w0 + w1;
    if (w == 0.0f)
        return 0.0f;

    /* (3.3.1) Lagrange multiplier
     *
     * Equation (4) was simplified because a single
     * constraint iteration is used (lambda = 0)
     */
    float dlambda = -C / (w + compliance / dt / dt);

    n = n * -dlambda;

    body0->applyCorrection(n, pos0, velocityLevel);
    body1->applyCorrection(-n, pos1, velocityLevel);

    return dlambda;
}

void XPBDSolver::debugContact(ContactSet* contact) {

    assert(contact != nullptr);

    XPBDSolver::p1->setPosition(contact->p1);
    XPBDSolver::p2->setPosition(contact->p2);

    XPBDSolver::n->setPosition(contact->p1);
    XPBDSolver::n->setRotation(QuatFromTwoVectors(vec3(0, 1.0f, 0), contact->n));

    XPBDSolver::r1->setPosition(contact->A->pose.p);
    XPBDSolver::r2->setPosition(contact->B->pose.p);
    XPBDSolver::r1->setRotation(contact->A->pose.q * QuatFromTwoVectors(vec3(0, 1.0f, 0), contact->r1));
    XPBDSolver::r2->setRotation(contact->B->pose.q * QuatFromTwoVectors(vec3(0, 1.0f, 0), contact->r2));
    XPBDSolver::r1->setScale(glm::length(contact->r1));
    XPBDSolver::r2->setScale(glm::length(contact->r2));

}
