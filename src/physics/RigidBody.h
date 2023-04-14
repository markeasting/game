#pragma once

#include "common.h"
#include "gfx/Mesh.h"
#include "physics/Collider.h"
#include "physics/Pose.h"

class RigidBody {
public:

    unsigned int id = 0;

    Pose pose = Pose();
    Pose prevPose = Pose();

    bool isDynamic = true;              // Whether physics applies to this body
    bool canCollide = true;             

    Ref<Mesh> mesh = nullptr;
    Ref<Collider> collider = nullptr;   // Physics representation of shape. https://en.m.wikipedia.org/wiki/Convex_hull

    vec3 vel = vec3(0);
    vec3 omega = vec3(0);               // https://en.m.wikipedia.org/wiki/Angular_velocity
    vec3 velPrev = vec3(0);
    vec3 omegaPrev = vec3(0);

    float invMass = 1.0f;
    vec3 invInertia = vec3(1.0f);       // https://en.m.wikipedia.org/wiki/Moment_of_inertia Tetrahedron of unit size with unit mass = 0.1331712

    vec3 force = vec3(0);
    vec3 torque = vec3(0);              // https://en.m.wikipedia.org/wiki/Torque

    float gravity = -9.81f;
    float staticFriction = 0.50f;       // https://en.m.wikipedia.org/wiki/Friction
    float dynamicFriction = 0.30f;      // https://en.m.wikipedia.org/wiki/Friction
    float bounciness = 0.4f;            // https://en.m.wikipedia.org/wiki/Coefficient_of_restitution

    RigidBody() = default;
    RigidBody(Ref<Mesh> mesh);
    RigidBody(Ref<Collider> collider, Ref<Mesh> mesh = nullptr);
    ~RigidBody() = default;

    void applyRotation(const vec3& rot, float scale = 1.0);
    void applyCorrection(const vec3& corr, const vec3& pos = vec3(0), bool velocityLevel = false);

    void integrate(const float &deltaTime);
    void update(const double &deltaTime);

    float getInverseMass(const vec3& normal, const vec3& pos = vec3(0)) const; // should also be const
    vec3 getVelocityAt(const vec3& pos, bool beforeSolve = false) const;
    vec3 localToWorld(const vec3& v);
    vec3 worldToLocal(const vec3& v);

    RigidBody setBox(const vec3& size, float density = 1.0);
    RigidBody setPosition(const vec3& pos);
    RigidBody setRotation(const quat& rot);
    RigidBody setColliderOffset(const vec3& offset);
    RigidBody makeStatic();
    RigidBody disableCollision();

    RigidBody applyForce(const vec3& force, const vec3& position = vec3(0));
    RigidBody applyTorque(const vec3& torque);

    void updateGeometry();
    
private:
    void updateCollider();
};
