
#include "entity/Car.h"

#include "gfx/Material.h"
#include "physics/XPBDSolver.h"

vec3 Wheel::NORMAL = vec3(0, -1.0f, 0);
vec3 Wheel::FORWARD = vec3(0, 0, 1.0f);
vec3 Wheel::RIGHT = vec3(-1.0f, 0, 0);

Car::Car(PhysicsHandler& phys): m_phys(phys) {

    auto lightDirection = ref<Uniform<vec3>>("u_lightDirection", vec3(0.5f, 0.0f, 2.0f));

    auto car = ref<Geometry>(obj::loadModelFromFile("assets/objects/car/car.obj"));
    auto car_wheel = ref<Geometry>(obj::loadModelFromFile("assets/objects/car/wheel.obj"));
    auto car_collider = ref<Geometry>(obj::loadModelFromFile("assets/objects/car/car_collider.obj"));
    auto colliderSize = vec3(1.62f, 0.84f, 3.2f);

    Material shadowMaterial = Material("Basic.vert", "BasicTextured.frag");
    shadowMaterial.assignTexture("assets/objects/car/shadow_body.png", "texture1");

    m_bodyShadow = ref<Mesh>(PlaneGeometry(1.0f, 2.0f, true), shadowMaterial);
    m_bodyShadow->setPosition({0, 1.0f, 0});
    m_bodyShadow->setScale(2.2f);

    m_body = ref<RigidBody>(
            ref<MeshCollider>(car_collider),
            ref<Mesh>(car, Material("Phong", { lightDirection }))
        );
        m_body->staticFriction = 0.1f;
        m_body->dynamicFriction = 0.1f;
        m_body->setBox(colliderSize, 210.0f);
        m_body->setPosition({ 4.0f, 2.0f, -3.0f });
        m_phys.add(m_body);

    for (size_t i = 0; i < 4; i++) {
        vec3 localPos = { 
            i % 2 == 0 ? 0.78 : -0.78, 
            -0.15, 
            i >= 2 ? -1.25473 : 1.11923  
        };

        /* Wheel */
        auto wheel = Wheel(ref<Mesh>(car_wheel, Material("Phong", { lightDirection, ref<Uniform<vec3>>("ambient", vec3(0.1)), ref<Uniform<vec3>>("diffuseAlbedo", vec3(0.1)) })));
        wheel.m_hardpoint = localPos;

        if (i >= 2) {
            wheel.m_driven = true;
            // wheel.m_damping = 350.0f;
            // wheel.m_grip = 1000.0f;
        }

        /* Debugging */
        Material colorMaterial = Material("Basic");
        colorMaterial.wireframe = true;
        wheel.m_origin = ref<Mesh>(BoxGeometry(0.2f), colorMaterial);
        wheel.m_line = ref<Mesh>(ArrowGeometry(), colorMaterial);
        wheel.m_line2 = ref<Mesh>(ArrowGeometry(), colorMaterial);

        m_wheels.push_back(wheel);
    }

}

Car Car::addTo(Ref<Layer> layer) {
    layer->add(m_body);

    layer->add(m_bodyShadow);

    for (auto& wheel : m_wheels) {
        layer->add(wheel.m_mesh);
        layer->add(wheel.m_origin);
        layer->add(wheel.m_line);
        layer->add(wheel.m_line2);
    }

    return *this;
}

void Car::applyThrottle(float throttle) {
    m_throttle = throttle;
}

void Car::applySteering(float steering) {
    m_steering = steering;
}

void Car::update(float dt) {

    // for (auto& c : m_tempConstraints) {
    //     m_phys.m_constraints.erase(m_phys.m_constraints.begin() + c);
    // }

    // m_phys.m_constraints.clear();

    for (size_t i = 0; i < 4; i++) {

        auto& wheel = m_wheels[i];

        vec3 hardpointW = m_body->localToWorld(wheel.m_hardpoint);
        // vec3 rayDir = m_body->pose.q * vec3(0, -1.0f, 0);

        auto [hit, N, dist, body] = m_phys.raycast(hardpointW, wheel.m_normal);
        vec3 localHitPos = m_body->worldToLocal(hit);

        vec3 F = wheel.update(
            m_body, 
            dist,
            m_throttle,
            m_steering,
            m_handbrake,
            dt
        );

        m_body->applyForce(F, hardpointW);

        // if (body) {
        //     /* Could be useful for static shadows */
        //     float projectionDist = glm::dot(N, hit - body->pose.p);
        //     vec3 point = hit - N * projectionDist;
        // }
    }

    /* Body shadow -- @TODO maybe use point-to-plane dist */
    auto [hit, N, dist, body] = m_phys.raycast(m_body->pose.p, m_wheels[0].m_normal);

    if (body) {
        float projectionDist = glm::dot(N, hit - body->pose.p);
        vec3 point = hit - N * (projectionDist - 0.01f);
        m_bodyShadow->setPosition(point);
        m_bodyShadow->setRotation(m_body->pose.q); // @TODO keep plane normal intact
    }
}
