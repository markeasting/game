
#include "entity/Car.h"

#include "gfx/Material.h"
#include "util/QuatFromTwoVectors.h"

vec3 Wheel::NORMAL = vec3(0, -1.0f, 0);
vec3 Wheel::FORWARD = vec3(0, 0, 1.0f);
vec3 Wheel::RIGHT = vec3(-1.0f, 0, 0);

vec3 Wheel::getSpringForce() {
    float Fspring = m_pos * m_stiffness;
    float Fdamping = m_damperVelocity * m_damping;

    m_springForce = Fspring + Fdamping;

    return m_springForce * m_normal;
}

vec3 Wheel::getSteeringForce(Ref<RigidBody> body, float dt) {

    float vx = glm::dot(m_worldVelocity, m_forward);
    float vy = glm::dot(m_worldVelocity, m_right);
    m_slipAngle = abs(vx) > 0.01f ? -atan(vy / abs(vx)) : 0.0f;

    /* Pacejka's Magic Formula - https://en.m.wikipedia.org/wiki/Hans_B._Pacejka */
    const float D = 0.050f;     /* Overall grip multiplier */
    const float B = 0.8f;       /* [0 - 2.0] Initial grip slope - (should be related to normal force) */
    const float C = 1.5f;       /* [0 - 2.0] Grip falloff / related to B. (< 1.0 means no falloff but never fully reaches max grip) */
    const float E = 0.5f;       /* [0 - 1.0] Falloff sharpness (lower is sharper) */
    const float Bx = B * m_slipAngle;
    float pacejka = D * sin(C * atan(Bx - E * (Bx - atan(Bx))));

    float lateralForce = m_springForce * pacejka;

    vec3 F = abs(vx) > 2.0f
        ? -lateralForce / dt * m_right /* F = ma */
        : -0.5f * vy / dt * m_right;

    return F;
}

vec3 Wheel::getDrivingForce(float throttle, bool handbrake) {

    bool isGoingForward = m_bodyVelocity > 0.0f;

    float drivingForce = throttle > 0 && !handbrake
        ? throttle * m_torque / m_radius
        : 0.0f;

    float brakeBias = m_driven ? 0.0f : 1.0f;
    float brakeSteerScale = 1.0f; //1.0f - min(abs(m_slipAngle) / 0.15f, 0.9f);
    float brakingForce = throttle < 0 
        ? throttle * brakeBias * brakeSteerScale * m_brakeTorque / m_radius
        : 0.0f;

    float engineBraking = isGoingForward 
        ? -0.15f * m_torque 
        : 0.15f * m_torque;

    float totalForce = m_driven 
        ? brakingForce + drivingForce + engineBraking
        : brakingForce;

    if (handbrake && m_driven && isGoingForward)
        totalForce = -2.0f * m_brakeTorque;

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

void Wheel::updateGeometry(Ref<RigidBody> body, float dt) {
    m_mesh->setPosition(
        body->localToWorld(
            m_hardpoint
        ) + (m_normal * m_pos)
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

void Wheel::debug(Ref<RigidBody> body) {
    m_origin->setPosition(body->localToWorld(m_hardpoint));
    m_origin->setRotation(body->pose.q);

    m_line2->setPosition(body->localToWorld(m_hardpoint));
    m_line2->setRotation(QuatFromTwoVectors(vec3(0, 1.0f, 0), m_right));

    m_line->setPosition(body->localToWorld(m_hardpoint));
    m_line->setRotation(QuatFromTwoVectors(vec3(0, 1.0f, 0), m_normal));
    m_line->setScale(m_springForce * 0.01f);
}

vec3 Wheel::update(
    Ref<RigidBody> body, 
    float groundDistance, 
    float throttle,
    float steering,
    bool handbrake,
    float bodyVelocity,
    float dt
) {

    m_worldVelocity = body->getVelocityAt(body->localToWorld(m_hardpoint));
    m_bodyVelocity = bodyVelocity;

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
    vec3 Fdrive = getDrivingForce(throttle, handbrake);
    vec3 Fsteer = getSteeringForce(body, dt);

    /* Constrain 'circle of grip' */
    vec3 Fcircle = Fsteer + Fdrive;
    if (glm::length(Fcircle) > m_grip)
        Fcircle = m_grip * glm::normalize(Fcircle);
    
    vec3 F = Fy + Fcircle;

    this->updateGeometry(body, dt);

    #ifdef DEBUG_MODE
        this->debug(body);
    #endif

    return this->isGrounded() ? F : vec3(0.0f);
}


Car::Car(PhysicsHandler& phys): m_phys(phys) {

    auto lightDirection = uniform("u_lightDirection", vec3(0.5f, 0.0f, 2.0f));

    auto car = ref<Geometry>(obj::loadModelFromFile("assets/objects/car/car.obj"));
    auto car_wheel = ref<Geometry>(obj::loadModelFromFile("assets/objects/car/wheel.obj"));
    auto car_collider = ref<Geometry>(obj::loadModelFromFile("assets/objects/car/car_collider.obj"));
    auto colliderSize = vec3(1.62f, 0.84f, 3.2f);

    Material shadowMaterial = Material("Basic.vert", "BasicTextured.frag", {
        uniform("u_opacity", 1.0f),
    });
    shadowMaterial.assignTexture("assets/objects/car/shadow_body.png", "texture1");

    m_bodyShadow = ref<Mesh>(PlaneGeometry(1.0f, 2.0f, true), shadowMaterial);
    m_bodyShadow->setPosition({0, 1.0f, 0});
    m_bodyShadow->setScale(2.2f);

    m_body = ref<RigidBody>(
            ref<MeshCollider>(car_collider),
            ref<Mesh>(car, Material("Phong", { lightDirection }))
            // ref<Mesh>(car, Material("Phong", { lightDirection, uniform("ambient", vec3(0, 0, 0.2)), uniform("diffuseAlbedo", vec3(0, 0, 0.7)) }))
        );
        m_body->name = "CarBody";
        m_body->canSleep = false;
        m_body->staticFriction = 0.1f;
        m_body->dynamicFriction = 0.005f;
        m_body->restitution = 0.2f;
        m_body->setBox(colliderSize, 210.0f);
        m_body->setPosition({ 4.0f, 2.0f, -3.0f });

    for (size_t i = 0; i < 4; i++) {
        vec3 localPos = { 
            i % 2 == 0 ? 0.78 : -0.78, 
            -0.15, 
            i >= 2 ? -1.25473 : 1.11923  
        };

        /* Wheel */
        auto wheel = Wheel(ref<Mesh>(car_wheel, Material("Phong", { lightDirection, uniform("ambient", vec3(0.1)), uniform("diffuseAlbedo", vec3(0.1)) })));
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

    m_phys.add(m_body);

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

    /* Calculate (forward) velocity */
    m_forward = m_body->pose.q * vec3(0, 0, 1.0f);
    m_velocity = glm::length(m_body->vel);

    if (glm::dot(m_body->vel, m_forward) < 0.0f)
        m_velocity = -m_velocity;

    /* Air resistance */
    const float Cd = 0.25f;
    const float A = 0.55f;
    float Fd = 0.5f * 1.293f * Cd * A * powf(m_velocity, 2.0f);

    m_body->applyForce(m_forward * -Fd, m_body->localToWorld(vec3(0)));

    /* Update wheels */
    for (size_t i = 0; i < 4; i++) {

        auto& wheel = m_wheels[i];

        vec3 hardpointW = m_body->localToWorld(wheel.m_hardpoint);

        auto [exists, point, normal, dist] = m_phys.raycast(hardpointW, wheel.m_normal);

        // if (i == 0) XPBDSolver::p1->setPosition(point);
        // if (i == 1) XPBDSolver::p2->setPosition(point);
        // if (i == 0) XPBDSolver::setDebugVector(wheel.m_normal, hardpointW);

        vec3 F = wheel.update(
            m_body, 
            dist,
            m_throttle,
            m_steering,
            m_handbrake,
            m_velocity,
            dt
        );

        if (exists)
            m_body->applyForce(F, hardpointW);
    }
    
    // if (glm::length(m_body->vel) > 0.5f) {
    //     vec3 localPos = { 0, 1.5f, -4.4f };
    //     float camDist = 4.4f;
    //     float camHeight = 1.5f;
    //     vec3 dir = glm::normalize(m_body->vel);
    //     vec3 trailer = glm::conjugate(m_body->pose.q) * -dir;

    //     m_camPos = m_body->localToWorld(trailer += localPos);
    //     m_camLookPos = m_body->localToWorld({ 0, 1.2f, 0 });
    // }

    m_camPos = m_body->localToWorld({ 0, 1.5f, -4.4f });
    m_camLookPos = m_body->localToWorld({ 0, 1.2f, 0 });

    /* Body shadow */
    auto [exists, point, normal, dist] = m_phys.raycast(m_body->pose.p, m_wheels[0].m_normal);

    if (exists) {
        vec3 offsetPoint = m_body->pose.p + (m_wheels[0].m_normal * (dist - 0.02f));
    
        m_bodyShadow->setPosition(offsetPoint);
        
        // @TODO should use normal, but add the car's forward direction (angleAxis?)
        // m_bodyShadow->setRotation(QuatFromTwoVectors(vec3(), normal)); 
        m_bodyShadow->setRotation(m_body->pose.q);

        m_bodyShadow->m_material->setUniform("u_opacity", 1.0f - (dist / 1.5f));
    }
}
