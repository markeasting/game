#pragma once

#include "scene/Layer.h"
#include "physics/PhysicsHandler.h"
#include "physics/RigidBody.h"

#include "util/QuatFromTwoVectors.h"

struct Wheel {
    
    vec3 m_hardpoint; /* Local space */

    static vec3 NORMAL;
    static vec3 FORWARD;
    static vec3 RIGHT;

    vec3 m_normal;
    vec3 m_forward = vec3(0, 0, 1.0f);
    vec3 m_right = vec3(1.0f, 0, 0);

    float m_pos;
    float m_prevPos;
    float m_velocity;
    float m_rotation = 0.0f;
    
    float m_radius = 0.25f;
    float m_springLength = 0.1f;

    float m_stiffness = 350.0f;
    float m_damping = 25.0f;
    float m_springForce = 0.0f;

    bool m_driven = false;
    float m_torque = 20.0f;

    float m_slipAngle = 0.0f;
    float m_grip = 25.0f;

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
        m_right = body->pose.q * -Wheel::RIGHT;
        m_forward = body->pose.q * Wheel::FORWARD;

        m_prevPos = m_pos;
        m_pos = groundDistance - m_radius;
        m_pos = clamp(m_pos, -m_springLength, m_springLength);

        m_velocity = (m_pos - m_prevPos) / dt;

        vec3 Fy = getSpringForce();
        vec3 Fsteer = getSteeringForce(steering, body, dt);
        vec3 Fdrive = getDrivingForce(throttle);

        vec3 Fcircle = Fsteer + Fdrive;
        if (glm::length(Fcircle) > m_grip)
            Fcircle = m_grip * glm::normalize(Fcircle);

        Log(glm::length(Fcircle));

        vec3 F = Fy + Fcircle;

        return m_pos < 0 ? F : vec3(0.0f);
    }

    vec3 getSpringForce() {
        m_springForce = (m_pos * m_stiffness) + (m_velocity * m_damping);
        return m_springForce * m_normal;
    }

    vec3 getSteeringForce(float steering, Ref<RigidBody> body, float dt) {
        auto vel = body->getVelocityAt(body->localToWorld(m_hardpoint));
        float v = glm::length(vel);

        m_rotation = !m_driven 
            ? steering * 0.5f * (1.0f - min(v / 35.0f, 0.9f))
            : 0.0f;
        m_right = glm::angleAxis(m_rotation, m_normal) * m_right;
        m_forward = glm::angleAxis(m_rotation, m_normal) * m_forward;

        float vx = glm::dot(vel, m_forward);
        float vy = glm::dot(vel, m_right);
        m_slipAngle = abs(vx) > 0.5f 
            ? -atan(vy / abs(vx)) 
            : -vy;

        /* Loosly based on Pacejka's Magic Formula */
        const float D = 0.2f;
        const float E = 2.5f; /* [0.5-2.5] */
        float lateralForce = D * m_springForce * sin(E * atan(E * atan(m_slipAngle)));
        
        vec3 F = - lateralForce / dt * m_right; /* F = ma, but without m LOL */

        return F;
    }

    vec3 getDrivingForce(float throttle) {
        return m_driven 
            ? m_torque * m_forward * clamp(throttle, -0.1f, 1.0f) 
            : m_torque * m_forward * clamp(throttle, -1.0f, 0.0f);
    }

    void updateGeometry(Ref<RigidBody> body) {
        m_mesh->setPosition(
            body->localToWorld(
                m_hardpoint + (m_normal * m_pos)
            )
        );
        // @TODO incorporate m_right;
        m_mesh->setRotation(body->pose.q);
    }

    void debug(Ref<RigidBody> body) {
        m_origin->setPosition(body->localToWorld(m_hardpoint));
        m_origin->setRotation(body->pose.q);

        m_line2->setPosition(body->localToWorld(m_hardpoint));
        m_line2->setRotation(QuatFromTwoVectors(vec3(0, 1.0f, 0), m_right));

        m_line->setPosition(body->localToWorld(m_hardpoint));
        m_line->setScale(-m_springForce * 0.1f);
        
    }
};

class Car {
public:

    vec3 m_camPos = { 0, 1.5f, -3.4f };
    vec3 m_camLookPos = { 0, 1.2f, 0 };
    
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
