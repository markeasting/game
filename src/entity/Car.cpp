
#include "entity/Car.h"

#include "gfx/Material.h"
#include "physics/XPBDSolver.h"

Car::Car(PhysicsHandler& phys): m_phys(phys) {

    auto lightDirection = ref<Uniform<vec3>>("u_lightDirection", vec3(0.5f, 0.0f, 2.0f));

    auto car = ref<Geometry>(obj::loadModelFromFile("assets/objects/car/car.obj"));
    auto car_wheel = ref<Geometry>(obj::loadModelFromFile("assets/objects/car/wheel.obj"));
    auto car_collider = ref<Geometry>(obj::loadModelFromFile("assets/objects/car/car_collider.obj"));
    auto colliderSize = vec3(1.42f, 0.95f, 3.0f);

    m_body = ref<RigidBody>(
            ref<MeshCollider>(car_collider),
            ref<Mesh>(car, Material("Phong", { lightDirection }))
        );
        m_body->setBox(colliderSize);
        m_body->setPosition({ 4.0f, 2.0f, -3.0f });
        m_phys.add(m_body);

    for (size_t i = 0; i < 4; i++) {
        vec3 localPos = { 
            i % 2 == 0 ? 0.598428 : -0.598428, 
            -0.163302 - 0.2, 
            i > 1 ? -0.984848 : 0.984848 
        };

        auto wheel = ref<RigidBody>(
                ref<BoxCollider>(0.3f),
                ref<Mesh>(car_wheel, Material("Phong", { lightDirection }))
            );
            wheel->disableCollision();
            wheel->setPosition(m_body->localToWorld(localPos));

        m_wheels.push_back(wheel);
        m_phys.add(wheel);

        auto c = ref<Constraint>();
        c->setStiffness(500.0f);
        c->setDamping(10.0f, 10.0f);
        c->setBodies(m_body, wheel, localPos, vec3(0.0f));

        m_constraints.push_back(c);
        m_phys.m_constraints.push_back(c);
    }

}

Car Car::addTo(Ref<Layer> layer) {
    layer->add(m_body);

    for (auto& wheel : m_wheels) 
        layer->add(wheel);

    return *this;
}

void Car::update(float dt) {

    // for (size_t i = 0; i < 4; i++) {

    //     auto& wheel = m_wheels[i];

    //     vec3 localPos = { 
    //         i % 2 == 0 ? 0.598428 : -0.598428, 
    //         -0.163302 - 0.2, 
    //         i > 1 ? -0.984848 : 0.984848 
    //     };

    //     vec3 dir = m_body->pose.q * vec3(0, -1.0f, 0); /* Must be normalized */

    //     float length = 0.6f;
    //     float dist = m_phys.raycast(m_body->localToWorld(localPos), dir);
    //     float x = dist - length;

    //     if (x > 0)
    //         continue;

    //     float F = 70 * x;
    //     m_body->applyForce(F * dir, m_body->localToWorld(localPos));

    // }

}
