#pragma once

#include "scene/Layer.h"
#include "physics/PhysicsHandler.h"
#include "physics/RigidBody.h"

#include "util/QuatFromTwoVectors.h"

struct Wheel {
    
    vec3 m_hardpoint;
    vec3 m_normal;

    static vec3 FORWARD;
    static vec3 RIGHT;

    vec3 m_forward = vec3(0, 0, 1.0f);
    vec3 m_right = vec3(1.0f, 0, 0);

    float m_pos;
    float m_prevPos;
    float m_velocity;
    float m_rotation = 0.0f;
    
    float m_radius = 0.25f;
    float m_springLength = 0.1f;

    float m_stiffness = 250.0f;
    float m_damping = 20.0f;
    float m_springForce = 0.0f;

    float m_torque = 20.0f;

    float m_slipAngle = 0.0f;
    float m_grip = 0.25f;

    Ref<Mesh> m_mesh;
    Ref<Mesh> m_origin;
    Ref<Mesh> m_line;

    Wheel(Ref<Mesh> mesh) : m_mesh(mesh) {};

    void update(Ref<RigidBody> body, vec3 rayDir, float groundDistance, float dt) {

        /* World space */
        m_right = body->pose.q * -Wheel::RIGHT;
        m_forward = body->pose.q * Wheel::FORWARD;
        m_normal = rayDir;

        m_prevPos = m_pos;
        m_pos = groundDistance - m_radius;

        if (m_pos > m_springLength)
            m_pos = m_springLength;

        if (m_pos < -m_springLength)
            m_pos = -m_springLength;

        m_velocity = (m_pos - m_prevPos) / dt;
    }

    vec3 getSpringForce() {

        if (m_pos > 0)
            return vec3(0.0f);

        m_springForce = (m_pos * m_stiffness) + (m_velocity * m_damping);

        return m_springForce * m_normal;
    }

    vec3 getSteeringForce(float steering, Ref<RigidBody> body, float dt) {

        if (m_pos > 0)
            return vec3(0.0f);

        auto vel = body->getVelocityAt(body->localToWorld(m_hardpoint));
        float v = glm::length(vel);

        m_rotation = steering * 0.6f * (1.0f - v / 25.0f);
        m_right = glm::angleAxis(m_rotation, m_normal) * m_right;

        float vt = glm::dot(vel, m_right);
        
        // @TODO simply use forward/right?
        m_slipAngle = vt / v;

        float a = v > 4.0f
            ? getGrip() / dt
            : vt * m_grip * 0.5 / dt;

        vec3 F = - (1.0f/body->invMass * a) * m_right;

        return F;
    }

    inline float getGrip() {
        float b = 1.5;
        return 0.75f * m_grip * (1.0f - pow((m_slipAngle-1.0f/b)*b, 2.0f));
    }

    vec3 getDrivingForce(float throttle) {
        if (m_pos > 0)
            return vec3(0.0f);

        return m_forward * m_torque * throttle;
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

        m_line->setPosition(body->localToWorld(m_hardpoint));
        m_line->setRotation(QuatFromTwoVectors(vec3(0, 1.0f, 0), m_right));
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
