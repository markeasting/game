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

    const float maxPhi = 0.5;
    const float phi = glm::length(rot);

    if (phi * scale > maxPhi)
        scale = maxPhi / phi;

    glm::quat dq = glm::quat(0.0f, rot.x * scale, rot.y * scale, rot.z * scale);
    dq = dq * this->pose.q;

    this->pose.q = glm::quat(
        this->pose.q.w + 0.5 * dq.w,
        this->pose.q.x + 0.5 * dq.x,
        this->pose.q.y + 0.5 * dq.y,
        this->pose.q.z + 0.5 * dq.z
    );
    this->pose.q = glm::normalize(this->pose.q);
}

void RigidBody::integrate(const float &dt) {

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
    this->vel += dt * this->force * this->invMass;
    this->pose.p += dt * this->vel;

    this->omega += dt * this->torque * this->invInertia;
    // this->pose.q += dt * 0.5 * glm::vec4(omega.x, omega.y, omega.z, 0) * this->pose.q;
    // this->pose.q = glm::normalize(this->pose.q);
    this->applyRotation(this->omega, dt);
}

void RigidBody::update(const double &dt) {
    
    if(!this->isDynamic) 
        return;

    this->velPrev = this->vel;
    this->omegaPrev = this->omega;

    this->vel = (this->pose.p - this->prevPose.p) / (float) dt;

    glm::quat dq = this->pose.q * glm::conjugate(this->prevPose.q);

    this->omega = vec3(dq.x * 2.0 / dt, dq.y * 2.0 / dt, dq.z * 2.0 / dt);

    if (dq.w < 0.0f)
        this->omega = vec3(-this->omega.x, -this->omega.y, -this->omega.z); // @TODO just omega = -omega?

    // // Dampening
    // this->vel = this->vel * (1.0f - 1.0f * (float) dt);
    // this->omega = this->omega * (1.0f - 1.0f * (float) dt);

    this->updateCollider();
}

void RigidBody::applyCorrection(const vec3& corr, const vec3& pos, bool velocityLevel) {

    if(!this->isDynamic) 
        return;

    vec3 dq = vec3(0.0f);

    if (glm::length(pos) < 0.00001f) { // @TODO check epsilon
        std::cout << "pos is zero!" << std::endl;
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
    vec3 dq2 = vec3(
        this->invInertia.x * dq.x,
        this->invInertia.y * dq.y,
        this->invInertia.z * dq.z
    );
    this->pose.rotate(dq2);

    if (velocityLevel)
        this->omega += dq2;
    else
        this->applyRotation(dq2);
}

RigidBody RigidBody::setBox(const vec3& size, float density) {

    float mass = size.x * size.y * size.z * density;
    this->invMass = 1.0 / mass;

    mass /= 12.0;
    this->invInertia.x = 1.0 / (size.y * size.y + size.z * size.z) / mass;
    this->invInertia.y = 1.0 / (size.z * size.z + size.x * size.x) / mass;
    this->invInertia.z = 1.0 / (size.x * size.x + size.y * size.y) / mass;

    return *this;
}

RigidBody RigidBody::setPosition(const vec3& position) {
    this->pose.p = position;

    this->updateCollider();

    return *this;
}

float RigidBody::getInverseMass(const vec3& normal, const vec3& pos) {

    vec3 n = vec3(0.0f);

    if (glm::length(pos) < 0.0001f) {
        // @TODO check epsilon
        std::cout << "pos is zero" << std::endl;
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
    vec3 vel = vec3(0.0f);

    if(this->isDynamic) {
        if (beforeSolve) {
            vel = this->velPrev + glm::cross(this->omegaPrev, (pos - this->prevPose.p));
        } else {
            vel = this->vel + glm::cross(this->omega, (pos - this->pose.p));
        }
    }

    return vel;
}

vec3 RigidBody::localToWorld(const vec3& v) {
    return (this->pose.q * v) + this->pose.p;
}

vec3 RigidBody::worldToLocal(const vec3& v) {
    return this->pose.q * (v - this->pose.p);
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

void RigidBody::setColliderOffset(const vec3& offset) {
    this->collider->setRelativePos(offset);
    this->updateCollider();
}

void RigidBody::updateCollider() {
    this->collider->updateGlobalPose(this->pose);
}
