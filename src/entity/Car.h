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

    float m_radius = 0.313f;
    float m_springLength = 0.1f;

    float m_caster = 0.03f;
    float m_camber = 0.05f;
    float m_rotation = 0.0f;

    float m_stiffness = 80000.0f;
    float m_damping = 3000.0f;

    bool m_driven = false;
    float m_torque = 1500.0f;

    float m_grip = 5000.0f;

    float m_pos; /* Readonly */
    float m_prevPos; /* Readonly */
    float m_slipAngle; /* Readonly */
    float m_springForce; /* Readonly */
    float m_springVelocity; /* Readonly */
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

        /* World space */
        m_normal = body->pose.q * Wheel::NORMAL;
        m_right = body->pose.q * Wheel::RIGHT;
        m_forward = body->pose.q * Wheel::FORWARD;

        m_worldVelocity = body->getVelocityAt(body->localToWorld(m_hardpoint));
        m_rotation = !m_driven 
            ? steering * 0.75f * (1.0f - min(glm::length(m_worldVelocity) / 35.0f, 0.9f))
            : 0.0f;

        m_prevPos = m_pos;
        m_pos = groundDistance - m_radius;
        m_pos = clamp(m_pos, -m_springLength, m_springLength);

        m_springVelocity = (m_pos - m_prevPos) / dt;
        m_right = glm::angleAxis(m_rotation, m_normal) * m_right;
        m_forward = glm::angleAxis(m_rotation, m_normal) * m_forward;

        vec3 Fy = getSpringForce();
        vec3 Fsteer = getSteeringForce(body, dt);
        vec3 Fdrive = getDrivingForce(throttle);

        vec3 Fcircle = Fsteer + Fdrive;
        if (glm::length(Fcircle) > m_grip)
            Fcircle = m_grip * glm::normalize(Fcircle);
        
        vec3 F = Fy + Fcircle;

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
        float Fdamping = m_springVelocity * m_damping;

        // if (m_pos < -m_springLength * 0.75f) {
        //     Fspring *= 1.5f;
        //     Fdamping *= 1.5f;
        // }

        m_springForce = Fspring + Fdamping;

        return m_springForce * m_normal;
    }

    inline vec3 getSteeringForce(Ref<RigidBody> body, float dt) {

        float vx = glm::dot(m_worldVelocity, m_forward);
        float vy = glm::dot(m_worldVelocity, m_right);
        m_slipAngle = abs(vx) > 0.05f ? -atan(vy / abs(vx)) : 0.0f;

        /* Loosly based on Pacejka's Magic Formula */
        const float D = 0.08f;
        const float E = 2.2f; /* [0.5-2.5] */
        float lateralForce = D * m_springForce * sin(E * atan(E * atan(m_slipAngle)));

        vec3 F = abs(vx) > 2.0f
            ? -lateralForce / dt * m_right /* F = ma, but without m LOL */
            : -100.0f * vy / dt * m_right;

        return F;
    }

    inline vec3 getDrivingForce(float throttle) {
        // @TODO check if driving backwards ;)
        return m_driven 
            ? (m_torque / m_radius) * m_forward * clamp(throttle, -0.5f, 1.0f) 
            : (m_torque / m_radius) * m_forward * clamp(throttle, -1.0f, 0.0f);
    }

    inline void updateGeometry(Ref<RigidBody> body, float dt) {
        m_mesh->setPosition(
            body->localToWorld(
                m_hardpoint + (m_normal * m_pos)
            )
        );
        // @TODO incorporate m_right;
        m_mesh->setRotation(body->pose.q);

        m_omega = this->isGrounded()
            ? glm::dot(m_worldVelocity, m_forward) / m_radius // @TODO already calculated forward vel in slip angle equation
            : m_omega *= 0.95;
        m_theta += m_omega * dt;

        quat dq = body->pose.q;
        dq = glm::angleAxis(-m_rotation, vec3(0, 1.0f, 0)) * dq;
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
        m_line->setScale(m_springForce * 0.001f);
        
    }
};

class Car {
public:

    vec3 m_camPos = { 0, 1.2f, 0 };
    vec3 m_camLookPos = { 0, 1.5f, -4.4f };
    
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

    std::vector<Ref<Constraint>> m_constraints;
    std::vector<int> m_tempConstraints;

};
