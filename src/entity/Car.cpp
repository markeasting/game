
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
            i % 2 == 0 ? 0.61 : -0.61, 
            -0.18, 
            i > 1 ? -0.89729 : 0.984848 
        };

        /* Wheel */
        auto wheel = Wheel(ref<Mesh>(car_wheel, Material("Phong", { lightDirection, ref<Uniform<vec3>>("ambient", vec3(0.1)), ref<Uniform<vec3>>("diffuseAlbedo", vec3(0.1)) })));
        wheel.m_hardpoint = localPos;

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

void Car::update(float dt) {

    // for (auto& c : m_tempConstraints) {
    //     m_phys.m_constraints.erase(m_phys.m_constraints.begin() + c);
    // }

    // m_phys.m_constraints.clear();

    for (size_t i = 0; i < 4; i++) {

        auto& wheel = m_wheels[i];

        vec3 hardpointW = m_body->localToWorld(wheel.m_hardpoint);
        vec3 rayDir = m_body->pose.q * vec3(0, -1.0f, 0); /* Must be normalized */

        auto [hit, N, dist, body] = m_phys.raycast(hardpointW, rayDir);
        vec3 localHitPos = m_body->worldToLocal(hit);

        wheel.update(rayDir, dist, dt);

        // if (body) {
        //     /* Could be useful for static shadows */
        //     float projectionDist = glm::dot(N, worldPos - body->pose.p);
        //     wheel->setPosition(worldPos - N * projectionDist);
        //     wheel->updateGeometry();
        // }

        vec3 F = wheel.getSpringForce();
        m_body->applyForce(F, hardpointW);

        wheel.updateGeometry(m_body);
        wheel.debug(m_body);
    }

}
