#include "physics/XPBDSolver.h"
#include "util/QuatFromTwoVectors.h"

void XPBDSolver::init() {
    Material colorMaterial = Material("Basic");
    colorMaterial.wireframe = true;

    XPBDSolver::p1 = ref<Mesh>(BoxGeometry(0.05f), colorMaterial);
    XPBDSolver::p2 = ref<Mesh>(BoxGeometry(0.05f), colorMaterial);
    XPBDSolver::r1 = ref<Mesh>(ArrowGeometry(1.0f), colorMaterial);
    XPBDSolver::r2 = ref<Mesh>(ArrowGeometry(1.0f), colorMaterial);
    XPBDSolver::n = ref<Mesh>(ArrowGeometry(1.0f), colorMaterial);

    XPBDSolver::debugArrow = ref<Mesh>(ArrowGeometry(1.0f), colorMaterial);
}

void XPBDSolver::update(const std::vector<Ref<RigidBody>>& bodies, const std::vector<Ref<Constraint>>& constraints, std::function<void(float)> customUpdate, const float dt) {

    /* XPBD algorithm 2 */

    /* (3.5)
     * To save computational cost we collect potential
     * collision pairs once per time step instead of once per
     * sub-step using a tree of axis aligned bounding boxes.
     */
    auto collisions = XPBDSolver::collectCollisionPairs(bodies, dt);

    Log(collisions.size());

    // const float h = dt / XPBDSolver::numSubSteps;
    const float h = (1.0f / 60.0f) / XPBDSolver::numSubSteps;

    for(int i = 0; i < XPBDSolver::numSubSteps; i++) {

        /* (3.5)
         * At each substep we iterate through the pairs
         * checking for actual collisions.
         */
        auto contacts = XPBDSolver::getContacts(collisions);

        for (auto const& body: bodies)
            body->integrate(h);

        for (auto const& constraint: constraints)
            constraint->solvePos(h);

        XPBDSolver::solvePositions(contacts, h);

        for (auto const& body: bodies)
            body->update(h);

        for (auto const& constraint: constraints)
            constraint->solveVel(h);

        XPBDSolver::solveVelocities(contacts, h);

        customUpdate(h);

    }

    for (auto const& body: bodies) {

        /* (3.5) k * dt * vbody */
        body->collider->m_expanded_aabb = AABB(body->collider->m_aabb);
        body->collider->m_expanded_aabb.expandByScalar(2.0f * dt * body->velocity);
        
        body->force = vec3(0.0f);
        body->torque = vec3(0.0f);
        body->updateGeometry();
    }
}

std::vector<CollisionPair> XPBDSolver::collectCollisionPairs(const std::vector<Ref<RigidBody>>& rigidBodies, const float dt) {

    // @TODO Chunking / octree
    // https://github.com/mwarning/SimpleOctree/tree/master/src

    std::vector<CollisionPair> collisions = {};

    for (int i = 0; i < rigidBodies.size(); i++) {
        auto const& A = rigidBodies[i];
        
        if (!A->canCollide)
            continue;

        for (int j = i + 1; j < rigidBodies.size(); j++) {
            auto const& B = rigidBodies[j];

            if (!B->canCollide)
                continue;

            if (!A->isDynamic && !B->isDynamic)
                continue;

            if (A == B)
                continue;

            const float vrel = glm::length(A->vel - B->vel);

            /* Wake sleeping bodies if a collision could occur */
            if (vrel > 0.1f) {
                A->wake();
                B->wake();
            }

            auto const& aabb1 = A->collider->m_expanded_aabb;
            auto const& aabb2 = B->collider->m_expanded_aabb;

            switch(A->collider->m_type) {
                case ColliderType::cMesh :
                    switch(B->collider->m_type) {
                        case ColliderType::cMesh : {

                            if (aabb1.intersects(aabb2))
                                collisions.push_back({ A.get(), B.get() });

                            break;
                        }
                        case ColliderType::cPlane : {

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

std::vector<Ref<ContactSet>> XPBDSolver::getContacts(const std::vector<CollisionPair>& collisions) {

    std::vector<Ref<ContactSet>> contacts = {};

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

                        auto contact = ref<ContactSet>(
                            A,
                            B,
                            -epa.normal,
                            epa.d,
                            epa.p1,
                            epa.p2,
                            A->worldToLocal(epa.p1),
                            B->worldToLocal(epa.p2)
                        );

                        contacts.push_back(contact);
                        // XPBDSolver::debugContact(contact);

                        break;
                    }

                    case ColliderType::cPlane : {

                        const auto& MC = static_cast<MeshCollider*>(A->collider.get());
                        const auto& PC = static_cast<PlaneCollider*>(B->collider.get());

                        const vec3 N = PC->m_plane.normal;

                        for(int i = 0; i < MC->m_uniqueIndices.size(); i++) {

                            /* (26) - p1 */
                            const vec3 r1 = MC->m_vertices[MC->m_uniqueIndices[i]];
                            const vec3 p1 = MC->m_verticesWorldSpace[MC->m_uniqueIndices[i]];

                            /**
                             * @TODO return some kind of 'hitinfo' from Plane class, 
                             * since most of these projection / distance functions
                             * use the same methods internally.
                             */
                            if (!PC->m_plane.containsPoint(p1))
                                continue;

                            /* (26) - p2 */
                            const vec3 p2 = PC->m_plane.projectPoint(p1);
                            const vec3 r2 = B->worldToLocal(p2);

                            /* (3.5) Penetration depth -- Note: sign was flipped! */
                            const float d = - glm::dot((p1 - p2), N);
                            // const float d = -PC->m_plane.distanceToPoint(p1); // This matches the calculation above!

                            /* (3.5) if d ≤ 0 we skip the contact */
                            if (d <= 0.0f)
                                continue;

                            auto contact = ref<ContactSet>(A, B, N, d, p1, p2, r1, r2);

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

void XPBDSolver::solvePositions(const std::vector<Ref<ContactSet>>& contacts, const float h) {

    /* (3.5) Handling contacts and friction */
    
    for (auto const& contact: contacts) {
        XPBDSolver::_solvePenetration(contact, h);
        XPBDSolver::_solveFriction(contact, h);
    }
}

void XPBDSolver::_solvePenetration(Ref<ContactSet> contact, const float h) {

    /* (26) - p1, p2 and penetration depth (d) are calculated here. */
    contact->update();

    /* (3.5) if d ≤ 0 we skip the contact */
    if(contact->d <= 0.0f)
        return;

    /* (3.5) Resolve penetration (Δx = dn using a = 0 and λn) */
    const vec3 dx = contact->d * contact->n;

    float delta_lambda = XPBDSolver::applyBodyPairCorrection(
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
    contact->lambda_n += delta_lambda;
}

void XPBDSolver::_solveFriction(Ref<ContactSet> contact, const float h) {

    /* (3.5) Static friction */
    contact->update();
    
    /* (26) Positions in current state and before the substep integration */
    const vec3 p1prev = contact->A->prevPose.p + contact->A->prevPose.q * contact->r1;
    const vec3 p2prev = contact->B->prevPose.p + contact->B->prevPose.q * contact->r2;

    /* (27) (28) Relative motion and tangential component */
    const vec3 dp = (contact->p1 - p1prev) - (contact->p2 - p2prev);
    vec3 dp_t = dp - glm::dot(dp, contact->n) * contact->n;
    
    /* Note: Had to negate dp_t to get correct results */
    dp_t = -dp_t;

    /* (3.5)
     * To enforce static friction, we apply Δx = Δp_t
     * at the contact points with a = 0.
     */
    const float d_lambda_t = XPBDSolver::applyBodyPairCorrection(
        contact->A,
        contact->B,
        dp_t,
        0.0f,
        h,
        contact->p1,
        contact->p2,
        false,
        true
    );

    /**
     * "...but only if λ_t < μ_s * λ_n"
     * 
     * Note: 
     *      this inequation was flipped because 
     *      the lambda values are always negative!
     * 
     * Note: 
     *      with 1 position iteration (XPBD), lambda_t is always zero!
     */ 
    if (contact->lambda_t + d_lambda_t > contact->staticFriction * contact->lambda_n) {
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

void XPBDSolver::solveVelocities(const std::vector<Ref<ContactSet>>& contacts, const float h) {

    /* (3.6) Velocity level */

    for (auto const& contact: contacts) {

        contact->update();

        vec3 dv = vec3(0.0f);

        /* (29) Relative normal and tangential velocities
         *
         * Note: v and vn are recalculated since the velocities were
         * modified by RigidBody::update() in the meantime.
         */
        const vec3 v = (
            contact->A->getVelocityAt(contact->p1) - 
            contact->B->getVelocityAt(contact->p2)
        );
        const float vn = glm::dot(contact->n, v);
        const vec3 vt = v - (contact->n * vn);
        const float vt_len = glm::length(vt);

        /* (30) Friction */
        if (vt_len > 0.0001f) {
            const float Fn = -contact->lambda_n / (h * h);
            const float friction = std::min(h * contact->dynamicFriction * Fn, vt_len);
            dv -= glm::normalize(vt) * friction;
        }

        /* (34) Restitution
         *
         * To avoid jittering we set e = 0 if vn is small (`threshold`).
         * 
         * Note: min() was replaced with max() due to the flipped sign convention.
         *
         * Note: `vn_tilde` is calculated in ContactSet before the position solve (Eq. 29)
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
    const float compliance,
    const float dt,
    const vec3& pos0,
    const vec3& pos1,
    const bool velocityLevel,
    const bool precalculateDeltaLambda
) {

    const float C = glm::length(corr);

    if ( C < 0.0001f )
        return 0.0f;

    vec3 n = glm::normalize(corr);

    float w0 = body0 ? body0->getInverseMass(n, pos0) : 0.0f;
    float w1 = body1 ? body1->getInverseMass(n, pos1) : 0.0f;

    float w = w0 + w1;
    if (w == 0.0f)
        return 0.0f;

    /* (3.3.1) Lagrange multiplier
     *
     * Equation (4) was simplified because a single
     * constraint iteration is used (initial lambda = 0)
     */
    float dlambda = -C / (w + compliance / dt / dt);

    if (!precalculateDeltaLambda) {
        n = n * -dlambda;

        if (body0) body0->applyCorrection(n, pos0, velocityLevel);
        if (body1) body1->applyCorrection(-n, pos1, velocityLevel);
    }

    return dlambda;
}

void XPBDSolver::debugContact(Ref<ContactSet> contact) {

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

void XPBDSolver::setDebugVector(const vec3& vector, const vec3& position) {
    XPBDSolver::debugArrow->setPosition(position);
    XPBDSolver::debugArrow->setScale(glm::length(vector));
    XPBDSolver::debugArrow->setRotation(QuatFromTwoVectors(vec3(0, 1.0f, 0), vector));
}
