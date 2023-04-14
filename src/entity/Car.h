#pragma once

#include "scene/Layer.h"
#include "physics/PhysicsHandler.h"
#include "physics/RigidBody.h"

#include "util/QuatFromTwoVectors.h"

struct Wheel {
    
    vec3 m_hardpoint;
    vec3 m_normal;

    // @TODO make static consts
    vec3 m_forward = vec3(0, 0, 1.0f);
    vec3 m_right = vec3(1.0f, 0, 0);

    float m_pos;
    float m_prevPos;
    float m_velocity;
    
    float m_radius = 0.25f;
    float m_springLength = 0.1f;

    float m_stiffness = 250.0f;
    float m_damping = 20.0f;

    Ref<Mesh> m_mesh;
    Ref<Mesh> m_origin;
    Ref<Mesh> m_line;

    Wheel(Ref<Mesh> mesh) : m_mesh(mesh) {};

    vec3 getSpringForce() {

        if (m_pos > 0)
            return vec3(0.0f);

        float F = (m_pos * m_stiffness) + (m_velocity * m_damping);

        return F * m_normal;
    }

    vec3 getSteeringForce(Ref<RigidBody> body, float dt) {

        vec3 _right = body->pose.q * m_right;

        auto v = body->getVelocityAt(body->localToWorld(m_hardpoint));

        float vt = glm::dot(v, _right) * 0.01f;

        vec3 F = - (1.0f/body->invMass * vt/dt) * _right;

        return F;
    }

    void update(vec3 rayDir, float groundDistance, float dt) {
        m_normal = rayDir;

        m_prevPos = m_pos;
        m_pos = groundDistance - m_radius;

        if (m_pos > m_springLength)
            m_pos = m_springLength;

        if (m_pos < -m_springLength)
            m_pos = -m_springLength;

        m_velocity = (m_pos - m_prevPos) / dt;
    }

    void updateGeometry(Ref<RigidBody> body) {
        m_mesh->setPosition(
            body->localToWorld(
                m_hardpoint + (m_normal * m_pos)
            )
        );
        m_mesh->setRotation(body->pose.q);
    }

    void debug(Ref<RigidBody> body) {
        m_origin->setPosition(body->localToWorld(m_hardpoint));
        m_origin->setRotation(body->pose.q);

        m_line->setPosition(body->localToWorld(m_right));
        m_line->setRotation(QuatFromTwoVectors(vec3(0, 1.0f, 0), m_right));
    }
};

class Car {
public:

    vec3 m_camPos = { 0, 1.5f, -3.4f };
    vec3 m_camLookPos = { 0, 1.2f, 0 };

    Car(PhysicsHandler& phys);
    virtual ~Car() = default;

    void update(float dt);

    Car addTo(Ref<Layer> layer);

// private:

    PhysicsHandler& m_phys;

    Ref<RigidBody> m_body;

    // std::vector<Ref<RigidBody>> m_wheels;
    std::vector<Wheel> m_wheels;

    std::vector<Ref<Constraint>> m_constraints;
    std::vector<int> m_tempConstraints;

};
