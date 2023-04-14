#pragma once

#include "scene/Layer.h"
#include "physics/PhysicsHandler.h"
#include "physics/RigidBody.h"

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

    std::vector<Ref<RigidBody>> m_wheels;

    std::vector<Ref<Constraint>> m_constraints;

};
