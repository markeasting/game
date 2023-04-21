#include "RigidBody.h"

RigidBody::RigidBody(Ref<Mesh> mesh)
    : mesh(mesh) 
{

    assert(mesh != nullptr);

    if(this->collider == nullptr) {
        this->collider = ref<MeshCollider>(mesh->m_geometry);
    }

    this->mesh->m_managedByRigidBody = true;
    this->pose.p = mesh->getPosition();
    this->pose.q = mesh->getRotation();

    this->updateCollider();
}

RigidBody::RigidBody(Ref<Collider> collider, Ref<Mesh> mesh)
    : collider(collider), mesh(mesh) 
{

    // if(this->collider == nullptr) {
    //     this->collider = ref<MeshCollider>(mesh->m_geometry);
    // }
    assert(collider != nullptr);

    if(mesh) {
        this->mesh->m_managedByRigidBody = true;
        this->pose.p = mesh->getPosition();
        this->pose.q = mesh->getRotation();
    }

    this->updateCollider();
}

RigidBody RigidBody::makeStatic() {
    this->isDynamic = false;
    this->gravity = 0.0f;
    this->invMass = 0.0f;
    this->invInertia = vec3(0.0f);

    this->updateGeometry();
    this->updateCollider();

    return *this;
}

RigidBody RigidBody::disableCollision() {
    this->canCollide = false;

    return *this;
}

RigidBody RigidBody::applyForce(const vec3& force, const vec3& position) {
    this->force += force;
    this->torque += glm::cross(force, (this->pose.p - position));

    return *this;
}

RigidBody RigidBody::applyTorque(const vec3& torque) {
    this->torque += torque;

    return *this;
}

void RigidBody::applyRotation(const vec3& rot, float scale) {

    // Safety clamping. This happens very rarely if the solver
    // wants to turn the body by more than 30 degrees in the
    // orders of milliseconds

    const float maxPhi = 0.5f;
    const float phi = glm::length(rot);

    if (phi * scale > maxPhi)
        scale = maxPhi / phi;

    glm::quat dq = glm::quat(
        0.0f, 
        rot.x * scale, 
        rot.y * scale, 
        rot.z * scale
    );
    dq = dq * this->pose.q;

    this->pose.q = glm::quat(
        this->pose.q.w + 0.5f * dq.w,
        this->pose.q.x + 0.5f * dq.x,
        this->pose.q.y + 0.5f * dq.y,
        this->pose.q.z + 0.5f * dq.z
    );
    this->pose.q = glm::normalize(this->pose.q);
}

void RigidBody::integrate(const float dt) {

    // Safety feature, can be removed later
    if (glm::all(glm::isnan(this->pose.p)) || glm::all(glm::isnan(this->pose.q)) || glm::all(glm::isnan(this->omega)) || glm::all(glm::isnan(this->vel))) {
        Log("ERR: RigidBody pose contains nan values");
        std::exit(EXIT_FAILURE);
    }

    if(!this->isDynamic) 
        return;

    this->prevPose.p = pose.p;
    this->prevPose.q = pose.q;

    // Euler step
    // this->vel += vec3(0, this->gravity, 0) * dt;
    // this->vel += this->force * this->invMass * dt;
    // this->omega += this->torque * this->invInertia * dt;
    // this->pose.p += this->vel * dt;
    // this->applyRotation(this->omega, dt);

    // Euler step (updated)
    this->vel += vec3(0, this->gravity, 0) * dt;
    this->vel += (this->force * this->invMass) * dt;
    this->pose.p += this->vel * dt;

    this->omega += (this->torque * this->invInertia) * dt;

    // this->pose.q += dt * 0.5 * glm::vec4(omega.x, omega.y, omega.z, 0) * this->pose.q;
    // this->pose.q = glm::normalize(this->pose.q);
    this->applyRotation(this->omega, dt);
}

void RigidBody::update(const float dt) {
    
    if(!this->isDynamic) 
        return;

    this->velPrev = this->vel;
    this->omegaPrev = this->omega;

    this->vel = (this->pose.p - this->prevPose.p) / dt;

    glm::quat dq = this->pose.q * glm::conjugate(this->prevPose.q);

    this->omega = vec3(
        dq.x * 2.0f / dt, 
        dq.y * 2.0f / dt, 
        dq.z * 2.0f / dt
    );

    if (dq.w < 0.0f)
        this->omega = vec3(-this->omega.x, -this->omega.y, -this->omega.z); // @TODO just omega = -omega?

    // // Dampening
    // this->vel = this->vel * (1.0f - 1.0f * dt);
    // this->omega = this->omega * (1.0f - 1.0f * dt);

    this->updateCollider();
}

void RigidBody::applyCorrection(const vec3& corr, const vec3& pos, bool velocityLevel) {

    if(!this->isDynamic) 
        return;

    vec3 dq = vec3(0.0f);

    if (glm::length(pos) < 0.00001f) { // @TODO check epsilon
        // std::cout << "pos is zero!" << std::endl;
        dq = corr;
    } else {
        if (velocityLevel)
            this->vel += corr * this->invMass;
        else
            this->pose.p += corr * this->invMass;
        dq = pos - this->pose.p;
        dq = glm::cross(dq, corr);
    }

    this->pose.invRotate(dq);
    dq = vec3(
        this->invInertia.x * dq.x,
        this->invInertia.y * dq.y,
        this->invInertia.z * dq.z
    );
    this->pose.rotate(dq);

    if (velocityLevel)
        this->omega += dq;
    else
        this->applyRotation(dq);
}

RigidBody RigidBody::setBox(const vec3& size, float density) {

    float mass = size.x * size.y * size.z * density;
    this->invMass = 1.0f / mass;

    mass /= 12.0f;
    this->invInertia.x = 1.0f / (size.y * size.y + size.z * size.z) / mass;
    this->invInertia.y = 1.0f / (size.z * size.z + size.x * size.x) / mass;
    this->invInertia.z = 1.0f / (size.x * size.x + size.y * size.y) / mass;

    return *this;
}

RigidBody RigidBody::setPosition(const vec3& position) {
    this->pose.p = position;

    this->updateCollider();

    return *this;
}

RigidBody RigidBody::setRotation(const quat& rotation) {
    this->pose.q = rotation;

    this->updateCollider();

    return *this;
}

RigidBody RigidBody::setColliderOffset(const vec3& offset) {
    this->collider->setRelativePos(offset);
    this->updateCollider();

    return *this;
}

float RigidBody::getInverseMass(const vec3& normal, const vec3& pos) const {

    if (!this->isDynamic)
        return 0.0f;

    vec3 n = vec3(0.0f);

    if (glm::length(pos) < 0.0001f) {
        // @TODO check epsilon
        // std::cout << "pos is zero" << std::endl;
        n = normal;
    } else {
        n = pos - this->pose.p;
        n = glm::cross(n, normal);
    }

    this->pose.invRotate(n);
    float w =
        n.x * n.x * this->invInertia.x +
        n.y * n.y * this->invInertia.y +
        n.z * n.z * this->invInertia.z;

    if (glm::length(pos) > 0.0001f)
        w += this->invMass;

    return w;
}

vec3 RigidBody::getVelocityAt(const vec3& pos, bool beforeSolve) const {

    if(!this->isDynamic)
        return vec3(0.0f);

    return this->vel + glm::cross(this->omega, (pos - this->pose.p));
}

vec3 RigidBody::localToWorld(const vec3& v) {
    return (this->pose.q * v) + this->pose.p;
}

vec3 RigidBody::worldToLocal(const vec3& v) {
    return glm::conjugate(this->pose.q) * (v - this->pose.p);
}

void RigidBody::updateGeometry() {
    if(this->mesh) {
        this->mesh->setPosition(this->pose.p);
        this->mesh->setRotation(this->pose.q);
    }

    if (this->collider->m_mesh != nullptr) {
        this->collider->m_mesh->setPosition(this->collider->m_relativePosW);
        this->collider->m_mesh->setRotation(this->pose.q);
    }
}

void RigidBody::updateCollider() {
    this->collider->updateGlobalPose(this->pose);
}
