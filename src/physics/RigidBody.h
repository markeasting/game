#pragma once

#include "common.h"
#include "gfx/Mesh.h"
#include "physics/Collider.h"
#include "physics/Pose.h"

class RigidBody {
public:

    unsigned int id = 0;
    std::string name = "body";

    Pose pose = Pose();                 /* Holds the current position and rotation */

    bool isDynamic = true;              /* Whether or not physics applies to this body */
    bool canCollide = true;             /* Whether or not this body can collide with other bodies */
    
    bool isSleeping = false;
    bool canSleep = true;
    float sleepTimer = 0.0f;

    Ref<Mesh> mesh = nullptr;           /* Visual representation of the body */
    Ref<Collider> collider = nullptr;   /* Physics representation of the body */

    vec3 vel = vec3(0);                 /* https://en.m.wikipedia.org/wiki/Velocity */
    vec3 omega = vec3(0);               /* https://en.m.wikipedia.org/wiki/Angular_velocity */

    float velocity() {
        return glm::length(this->vel);
    };

    float invMass = 1.0f;               /* https://en.m.wikipedia.org/wiki/Mass */
    vec3 invInertia = vec3(1.0f);       /* https://en.m.wikipedia.org/wiki/Moment_of_inertia */

    vec3 force = vec3(0);               /* https://en.m.wikipedia.org/wiki/Force */
    vec3 torque = vec3(0);              /* https://en.m.wikipedia.org/wiki/Torque */
    float gravity = -9.81f;             /* https://en.m.wikipedia.org/wiki/Gravity */

    float staticFriction = 0.5f;        /* https://en.m.wikipedia.org/wiki/Friction */
    float dynamicFriction = 0.4f;       /* https://en.m.wikipedia.org/wiki/Friction */
    float restitution = 0.4f;           /* https://en.m.wikipedia.org/wiki/Coefficient_of_restitution */

    RigidBody() = default;
    RigidBody(Ref<Mesh> mesh, bool isConvex = true);
    RigidBody(Ref<Collider> collider, Ref<Mesh> mesh = nullptr);
    ~RigidBody() = default;

    // @TODO setPos, setRotation, setVel, setOmega, etc.
    RigidBody setMesh(Ref<Mesh> mesh, bool applyTransform = true);
    RigidBody setBox(const vec3& size, float density = 1.0);
    RigidBody setPosition(const vec3& pos);
    RigidBody setRotation(const quat& rot);
    RigidBody setColliderOffset(const vec3& offset);
    RigidBody makeStatic();
    RigidBody disableCollision();

    RigidBody applyForce(const vec3& force, const vec3& position = vec3(0));
    RigidBody applyTorque(const vec3& torque);

    float getInverseMass(const vec3& normal, const vec3& pos = vec3(0)) const; // should also be const
    vec3 getVelocityAt(const vec3& pos, bool beforeSolve = false) const;
    vec3 localToWorld(const vec3& v);
    vec3 worldToLocal(const vec3& v);

    void integrate(const float deltaTime);
    void update(const float deltaTime);

    void checkSleepState(float dt);
    void sleep();
    void wake();

    void updateGeometry();
    void updateCollider();

    void applyRotation(const vec3& rot, float scale = 1.0);
    void applyCorrection(const vec3& corr, const vec3& pos = vec3(0), bool velocityLevel = false);

// private:

    // @TODO make private with friend class at some point
    vec3 velPrev = vec3(0);
    vec3 omegaPrev = vec3(0);
    Pose prevPose = Pose();
};
