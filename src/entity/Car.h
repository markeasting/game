#pragma once

#include "engine/scene/Layer.h"
#include "engine/physics/PhysicsHandler.h"
#include "engine/physics/RigidBody.h"

class Wheel {
public:

    static vec3 NORMAL;
    static vec3 FORWARD;
    static vec3 RIGHT;
    
    /* Local space */
    vec3 m_hardpoint;

    /* World space */
    vec3 m_normal;
    vec3 m_forward;     
    vec3 m_right;

    vec3 m_worldVelocity;
    float m_bodyVelocity = 0.0f;

    float m_radius = 0.313f;
    float m_springLength = 0.08f;

    float m_steerAngle = 0.0f;
    float m_caster = 0.03f; // @TODO
    float m_camber = 0.05f; // @TODO

    float m_stiffness = 12500.0f;
    float m_damping = 750.0f;

    bool m_driven = false;
    float m_torque = 200.0f;
    float m_brakeTorque = 400.0f;

    float m_grip = 1100.0f;

protected:
    float m_pos; /* Readonly */
    float m_prevPos; /* Readonly */
    float m_slipAngle; /* Readonly */
    float m_springForce; /* Readonly */
    float m_damperVelocity; /* Readonly */
    float m_omega;
    float m_theta;

public:
    Ref<Mesh> m_mesh;
    Ref<Mesh> m_origin;
    Ref<Mesh> m_line;
    Ref<Mesh> m_line2;

    Wheel(Ref<Mesh> mesh) : m_mesh(mesh) {};

    vec3 update(
        Ref<RigidBody> body, 
        float groundDistance, 
        float throttle,
        float steering,
        bool handbrake,
        float bodyVelocity,
        float dt
    );

private:
    
    inline bool isGrounded() { return m_pos < 0.0f; }

    vec3 getSpringForce();

    vec3 getSteeringForce(Ref<RigidBody> body, float dt);

    vec3 getDrivingForce(float throttle, bool handbrake);

    void updateGeometry(Ref<RigidBody> body, float dt);

    void debug(Ref<RigidBody> body);
};

class Car {
public:

    vec3 m_camLookPos = { 0, 1.2f, 0 };
    vec3 m_camPos = { 0, 1.5f, -4.4f };

    vec3 m_forward;
    
    float m_throttle = 0.0f;
    float m_steering = 0.0f;
    bool m_handbrake = false;

    float m_velocity;

    Car(PhysicsHandler& phys);
    virtual ~Car() = default;

    void update(float dt);

    Car addTo(Ref<Layer> layer);

    void applyThrottle(float throttle);
    void applySteering(float steering);

// private:

    PhysicsHandler& m_phys;

    Ref<RigidBody> m_body;

    // std::vector<Ref<RigidBody>> m_wheels;
    std::vector<Wheel> m_wheels;

    Ref<Mesh> m_bodyShadow;

    std::vector<Ref<Constraint>> m_constraints;
    std::vector<int> m_tempConstraints;

};
