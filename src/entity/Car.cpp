
#include "entity/Car.h"

#include "gfx/Material.h"
#include "physics/XPBDSolver.h"

vec3 Wheel::NORMAL = vec3(0, -1.0f, 0);
vec3 Wheel::FORWARD = vec3(0, 0, 1.0f);
vec3 Wheel::RIGHT = vec3(1.0f, 0, 0);

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
            i % 2 == 0 ? 0.61 : -0.61, 
            -0.18, 
            i >= 2 ? -0.89729 : 0.984848 
        };

        /* Wheel */
        auto wheel = Wheel(ref<Mesh>(car_wheel, Material("Phong", { lightDirection, ref<Uniform<vec3>>("ambient", vec3(0.1)), ref<Uniform<vec3>>("diffuseAlbedo", vec3(0.1)) })));
        wheel.m_hardpoint = localPos;

        if (i >= 2) {
            wheel.m_driven = true;
            // wheel.m_grip = 15.0f;
        }

        /* Debugging */
        Material colorMaterial = Material("Basic");
        colorMaterial.wireframe = true;
        wheel.m_origin = ref<Mesh>(BoxGeometry(0.2f), colorMaterial);
        wheel.m_line = ref<Mesh>(ArrowGeometry(), colorMaterial);

        m_wheels.push_back(wheel);
    }

}

Car Car::addTo(Ref<Layer> layer) {
    layer->add(m_body);

    for (auto& wheel : m_wheels) {
        layer->add(wheel.m_mesh);
        layer->add(wheel.m_origin);
        layer->add(wheel.m_line);
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
            dt
        );

        // if (body) {
        //     /* Could be useful for static shadows */
        //     float projectionDist = glm::dot(N, worldPos - body->pose.p);
        //     wheel->setPosition(worldPos - N * projectionDist);
        //     wheel->updateGeometry();
        // }

        // m_body->applyForce(wheel.getSpringForce(), hardpointW);

        // // @TODO set 'driven' property in wheel
        // if (i >= 2) 
        //     m_body->applyForce(wheel.getDrivingForce(m_throttle), hardpointW);

        // m_body->applyForce(wheel.getSteeringForce(i >= 2 ? 0 : m_steering, m_body, dt), hardpointW);
        
        m_body->applyForce(F, hardpointW);

        wheel.updateGeometry(m_body);
        wheel.debug(m_body);
    }

    // m_throttle = 0.0f;
    // m_steering = 0.0f;

}
