#pragma once

#include "scene/Layer.h"
#include "physics/PhysicsHandler.h"
#include "physics/RigidBody.h"

#include "util/QuatFromTwoVectors.h"

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
    float m_springLength = 0.1f;

    float m_caster = 0.03f;
    float m_camber = 0.05f;
    float m_steerAngle = 0.0f;

    float m_stiffness = 12000.0f;
    float m_damping = 470.0f;

    bool m_driven = false;
    float m_torque = 200.0f;
    float m_brakeTorque = 400.0f;

    float m_grip = 1100.0f;

    float m_pos; /* Readonly */
    float m_prevPos; /* Readonly */
    float m_slipAngle; /* Readonly */
    float m_springForce; /* Readonly */
    float m_damperVelocity; /* Readonly */
    float m_omega;
    float m_theta;

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
        float dt
    ) {

        m_worldVelocity = body->getVelocityAt(body->localToWorld(m_hardpoint));
        m_bodyVelocity = glm::length(body->vel);

        /* World space */
        m_normal = body->pose.q * Wheel::NORMAL;
        m_right = body->pose.q * Wheel::RIGHT;
        m_forward = body->pose.q * Wheel::FORWARD;

        /* Steering with speed scaling */
        m_steerAngle = !m_driven 
            ? steering * 0.65f * (1.0f - min(m_bodyVelocity / 40.0f, 0.90f))
            : 0.0f;

        /* Scale steering with throttle input */
        // m_steerAngle *= (m_bodyVelocity > 10.0f && throttle < 0) ? (1.0f + throttle) : 1.0f;

        m_right = glm::angleAxis(m_steerAngle, m_normal) * m_right;
        m_forward = glm::angleAxis(m_steerAngle, m_normal) * m_forward;

        /* Integrate */
        m_prevPos = m_pos;
        m_pos = groundDistance - m_radius;
        m_pos = clamp(m_pos, -m_springLength, m_springLength);
        m_damperVelocity = (m_pos - m_prevPos) / dt;

        /* Forces */
        // @TODO braking force should always be in non-steered forward direction?
        vec3 Fy = getSpringForce();
        vec3 Fdrive = getDrivingForce(throttle);
        vec3 Fsteer = getSteeringForce(body, dt);

        /* Constrain 'circle of grip' */
        vec3 Fcircle = Fsteer + Fdrive;
        if (glm::length(Fcircle) > m_grip)
            Fcircle = m_grip * glm::normalize(Fcircle);
        
        vec3 F = Fy + Fcircle;

        // if (m_driven)
        //     Log(glm::length(Fcircle));

        this->updateGeometry(body, dt);

        #ifndef NDEBUG
            this->debug(body);
        #endif

        return this->isGrounded() ? F : vec3(0.0f);
    }

private:
    
    inline bool isGrounded() {
        return m_pos < 0.0f;
    }

    inline vec3 getSpringForce() {
        float Fspring = m_pos * m_stiffness;
        float Fdamping = m_damperVelocity * m_damping;

        m_springForce = Fspring + Fdamping;

        return m_springForce * m_normal;
    }

    inline vec3 getSteeringForce(Ref<RigidBody> body, float dt) {

        float vx = glm::dot(m_worldVelocity, m_forward);
        float vy = glm::dot(m_worldVelocity, m_right);
        m_slipAngle = abs(vx) > 0.05f ? -atan(vy / abs(vx)) : 0.0f;

        /* Loosly based on Pacejka's Magic Formula */
        const float D = 0.025f;
        const float E = 2.1f; /* [0.5-2.5] */
        float lateralForce = D * m_springForce * sin(E * atan(E * atan(m_slipAngle)));

        vec3 F = abs(vx) > 2.0f
            ? -lateralForce / dt * m_right /* F = ma */
            : -0.5f * vy / dt * m_right;

        return F;
    }

    inline vec3 getDrivingForce(float throttle) {

        float drivingForce = throttle > 0
            ? throttle * m_torque / m_radius
            : 0.0f;

        float brakeBias = m_driven ? 0.0f : 1.0f;
        float brakeSteerScale = 1.0f; //1.0f - min(abs(m_slipAngle) / 0.15f, 0.9f);
        float brakingForce = throttle < 0 
            ? throttle * brakeBias * brakeSteerScale * m_brakeTorque / m_radius
            : 0.0f;

        float engineBraking = m_bodyVelocity > 2.0f 
            ? -0.15f * m_torque 
            : 0.0f;

        float totalForce = m_driven 
            ? brakingForce + drivingForce + engineBraking
            : brakingForce;

        /* Loosly based on Pacejka's Magic Formula */
        // const float D = 50.0f;
        // const float E = 2.9f; /* [0.5-2.5] */ /* Note: there is some relation to E with spring force. */
        // const float slipRatio = (m_omega*m_radius/m_bodyVelocity - 1.0f) * 100.0f;
        // float gripForceRatio = abs(D * (m_springForce / m_stiffness) * sin(E * atan(E * atan(slipRatio))));
        // Log(gripForceRatio);

        // if (m_FORWARD_and_not_body_total_velocity < 0.0f)
        //     totalForce = -totalForce;

        return totalForce * m_forward;
    }

    inline void updateGeometry(Ref<RigidBody> body, float dt) {
        m_mesh->setPosition(
            body->localToWorld(
                m_hardpoint + (m_normal * m_pos)
            )
        );
        m_mesh->setRotation(body->pose.q);

        m_omega = this->isGrounded()
            ? glm::dot(m_worldVelocity, m_forward) / m_radius // @TODO already calculated forward vel in slip angle equation
            : m_omega *= 0.95;
        m_theta += m_omega * dt;

        quat dq = body->pose.q;
        dq = glm::angleAxis(-m_steerAngle, vec3(0, 1.0f, 0)) * dq;
        dq = dq * glm::angleAxis(m_theta, vec3(1.0f, 0, 0));

        m_mesh->setRotation(dq);
    }

    void debug(Ref<RigidBody> body) {
        m_origin->setPosition(body->localToWorld(m_hardpoint));
        m_origin->setRotation(body->pose.q);

        m_line2->setPosition(body->localToWorld(m_hardpoint));
        m_line2->setRotation(QuatFromTwoVectors(vec3(0, 1.0f, 0), m_right));

        m_line->setPosition(body->localToWorld(m_hardpoint));
        m_line->setRotation(QuatFromTwoVectors(vec3(0, 1.0f, 0), m_normal));
        m_line->setScale(m_springForce * 0.01f);
    }
};

class Car {
public:

    vec3 m_camLookPos = { 0, 1.2f, 0 };
    vec3 m_camPos = { 0, 1.5f, -4.4f };
    
    float m_throttle;
    float m_steering;

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
