#include "physics/XPBDSolver.h"

void XPBDSolver::update(const std::vector<Ref<RigidBody>>& bodies, const float& dt) {

    /* XPBD algorithm 2 */

    /* (3.5)
     * To save computational cost we collect potential
     * collision pairs once per time step instead of once per
     * sub-step using a tree of axis aligned bounding boxes.
     */
    auto collisions = XPBDSolver::collectCollisionPairs(bodies, dt); // AABBs

    // const double h = dt / XPBDSolver::numSubSteps;
    const double h = (1.0f / 60.0f) / XPBDSolver::numSubSteps;

    for(int i = 0; i < XPBDSolver::numSubSteps; i++) {

        /* (3.5)
         * At each substep we iterate through the pairs
         * checking for actual collisions.
         */
        auto contacts = XPBDSolver::getContacts(collisions);

        for (auto& body: bodies) {
            body->integrate(h);
        }

        XPBDSolver::solvePositions(contacts, h);

        for (auto& body: bodies) {
            body->update(h);
        }

        XPBDSolver::solveVelocities(contacts, h);

    }

    // @TODO move to start of body->integrate?
    for (auto& body: bodies) {
        body->force = vec3(0.0f);
        body->torque = vec3(0.0f);
        body->updateGeometry();
    }
}

std::vector<CollisionPair> XPBDSolver::collectCollisionPairs(const std::vector<Ref<RigidBody>>& rigidBodies, const float& dt) {

    // @TODO Chunking / octree
    // https://github.com/mwarning/SimpleOctree/tree/master/src

    // @TODO Broad phase collision detection using bounding boxes

    std::vector<CollisionPair> collisions = {};

    std::vector<std::string> pairs = {};

    for (auto A: rigidBodies) {
        for (auto B: rigidBodies) {

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
            const float collisionMargin = 2.0f * (float) dt * glm::length(A->vel - B->vel);

            switch(A->collider->m_type) {
                case ColliderType::ConvexMesh :
                    switch(B->collider->m_type) {
                        case ColliderType::Plane : {

                            // @TODO use a different / better cast?
                            const auto MC = static_cast<MeshCollider*>(A->collider.get());
                            const auto PC = static_cast<PlaneCollider*>(B->collider.get());
                            const vec3& N = glm::normalize(PC->m_normal);

                            float deepestPenetration = 0.0f;

                            // This should be a simple AABB check instead of actual loop over all m_vertices
                            for(int i = 0; i < MC->m_uniqueIndices.size(); i++) {
                                const Vertex& v = MC->m_vertices[MC->m_uniqueIndices[i]];
                                
                                // vec3 contactPointW = CoordinateSystem::localToWorld(v.position, A->pose.q, A->pose.p);
                                vec3 contactPointW = A->localToWorld(v.position);
                                const float signedDistance = glm::dot(N, (contactPointW - B->pose.p)); // Simple point-to-plane dist

                                deepestPenetration = std::min(deepestPenetration, signedDistance);
                            }

                            if(deepestPenetration < collisionMargin) {
                                collisions.push_back({ A.get(), B.get() });
                            }

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

    for (auto collision: collisions) {

        RigidBody* A = collision.A;
        RigidBody* B = collision.B;

        switch(A->collider->m_type) {
            case ColliderType::ConvexMesh :
                switch(B->collider->m_type) {
                    case ColliderType::Plane : {

                        const auto MC = static_cast<MeshCollider*>(A->collider.get());
                        const auto PC = static_cast<PlaneCollider*>(B->collider.get());

                        // @TODO store some kind of plane object in PlaneCollider
                        // @TODO check if vertex is actually inside plane size :)
                        const vec3& N = PC->m_normal;
                        // const float C = glm::dot(B->pose.p, N); // plane constant (dist from origin)

                        for(int i = 0; i < MC->m_uniqueIndices.size(); i++) {
                            const Vertex& v = MC->m_vertices[MC->m_uniqueIndices[i]];
                            // const vec3 point = CoordinateSystem::localToWorld(v.position, A->pose.q, A->pose.p);

                            /* (26) - p1 */
                            const vec3 r1 = v.position;
                            const vec3 p1 = A->pose.p + A->pose.q * r1;

                            /* (26) - p2 */
                            const float signedDistance = glm::dot(N, p1 - B->pose.p);
                            const vec3 p2 = p1 - (N * signedDistance);
                            // const vec3 r2 = CoordinateSystem::worldToLocal(p2, B->pose.q, B->pose.p);
                            const vec3 r2 = B->worldToLocal(p2);

                            /* (3.5) Penetration depth -- Note: sign was flipped! */
                            // const float d = - glm::dot((p1 - p2), N);
                            const float d = -signedDistance; // This matches the calculation above!

                            /* (3.5) if d ≤ 0 we skip the contact */
                            if (d <= 0.0f)
                                continue;

                            ContactSet* contact = new ContactSet(A, B);

                            contact->d = d;
                            contact->n = N;
                            contact->p1 = p1;
                            contact->p2 = p2;
                            contact->r1 = r1;
                            contact->r2 = r2;

                            /*
                             * (29) Relative velocity
                             * This is calculated before the velocity solver,
                             * so that we can solve restitution (Eq. 34).
                             * 
                             * ṽn == contact->vn
                             */
                            contact->vrel = (
                                A->getVelocityAt(contact->p1) - 
                                B->getVelocityAt(contact->p2)
                            );
                            contact->vn = glm::dot(contact->n, contact->vrel);

                            contact->e = 0.5f * (A->bounciness + B->bounciness);
                            contact->staticFriction = 0.5f * (A->staticFriction + B->staticFriction);
                            contact->dynamicFriction = 0.5f * (A->dynamicFriction + B->dynamicFriction);

                            contacts.push_back(contact);
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

    for (auto contact: contacts) {
        /* (3.5) Handling contacts and friction */
        XPBDSolver::_solvePenetration(contact, h);
        XPBDSolver::_solveFriction(contact, h);
    }
}

void XPBDSolver::_solvePenetration(ContactSet* contact, const float& h) {

    auto A = contact->A;
    auto B = contact->B;

    /* (26) - p1 & p2 */
    contact->p1 = A->pose.p + A->pose.q * contact->r1;
    contact->p2 = B->pose.p + B->pose.q * contact->r2;

    /* (3.5) Penetration depth -- Note: sign was flipped! */
    contact->d = - glm::dot((contact->p1 - contact->p2), contact->n);

    /* (3.5) if d ≤ 0 we skip the contact */
    if(contact->d <= 0.0f)
        return;

    /* (3.5) Resolve penetration (Δx = dn using a = 0 and λn) */
    const vec3 dx = contact->d * contact->n;

    float dlambda = XPBDSolver::applyBodyPairCorrection(
        A,
        B,
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

    auto A = contact->A;
    auto B = contact->B;

    /* (3.5)
     * To handle static friction we compute the relative
     * motion of the contact points and its tangential component
     */

    /* (26) Positions in current state and before the substep integration */
    const vec3 p1prev = contact->p1; //A->prevPose.p + A->prevPose.q * contact->r1;
    const vec3 p2prev = contact->p2; //B->prevPose.p + B->prevPose.q * contact->r2;
    const vec3 p1 = A->pose.p + A->pose.q * contact->r1;
    const vec3 p2 = B->pose.p + B->pose.q * contact->r2;

    /* (27) (28) Relative motion and tangential component */
    const vec3 dp = (p1 - p1prev) - (p2 - p2prev);
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
            A,
            B,
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

    for (auto contact: contacts) {

        // Update contact positions
        const vec3 p1 = contact->A->pose.p + contact->A->pose.q * contact->r1;
        const vec3 p2 = contact->B->pose.p + contact->B->pose.q * contact->r2;

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