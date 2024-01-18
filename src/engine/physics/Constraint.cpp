
#include "engine/physics/Constraint.h"
#include "engine/physics/XPBDSolver.h"

void Constraint::setBodies(
    Ref<RigidBody> body0, 
    Ref<RigidBody> body1, 
    Pose localPose0, 
    Pose localPose1
) {
    m_body0 = body0;
    m_body1 = body1;

    m_localPose0 = localPose0; //.clone();
    m_localPose1 = localPose1; //.clone();

    m_globalPose0 = localPose0; //.clone();
    m_globalPose1 = localPose1; //.clone();
}

void Constraint::setBodies(
    Ref<RigidBody> body0, 
    Ref<RigidBody> body1, 
    vec3 localPose0, 
    vec3 localPose1
) {
    this->setBodies(body0, body1, Pose(localPose0), Pose(localPose1));
}

// m/N
void Constraint::setCompliance(float compliance) {
    m_compliance = compliance;
}

// N/m
void Constraint::setStiffness(float stiffness) {
    m_compliance = 1.0f / stiffness;
}

void Constraint::setDamping(float posDamping, float rotDamping) {
    m_posDamping = posDamping;
    m_rotDamping = rotDamping;
}

vec3 Constraint::getForce(float h) const {
    return (m_normal * m_lambda) / (h * h);
}

void Constraint::updateGlobalPoses() {
    m_globalPose0.copy(m_localPose0);
    m_globalPose1.copy(m_localPose1);

    if (m_body0)
        m_body0->pose.transformPose(m_globalPose0);
    if (m_body1)
        m_body1->pose.transformPose(m_globalPose1);
}

void Constraint::solvePos(float h) {

    this->updateGlobalPoses();

    // @TODO generalize creating C (constraint error) and it's normal
    const vec3 corr = m_globalPose1.p - m_globalPose0.p;
    
    if (glm::length(corr) < 0.0001f)
        return;

    m_normal = glm::normalize(corr);

    m_lambda = XPBDSolver::applyBodyPairCorrection(
        m_body0.get(),
        m_body1.get(),
        corr,
        m_compliance,
        h,
        m_globalPose0.p,
        m_globalPose1.p
    );

}

void Constraint::solveVel(float h) {

    if (m_rotDamping > 0.0f) {
        vec3 omega = vec3(0.0f);
        
        if (m_body0)
            omega -= m_body0->omega;
        if (m_body1)
            omega += m_body1->omega;

        omega *= std::min(1.0f, m_rotDamping * h);

        XPBDSolver::applyBodyPairCorrection(
            m_body0.get(),
            m_body1.get(),
            omega,
            0.0f,
            h,
            vec3(0),
            vec3(0),
            true
        );
    }

    if (m_posDamping > 0.0f) {
        this->updateGlobalPoses();
        
        vec3 vel = vec3(0.0f);

        if (m_body0)
            vel -= m_body0->getVelocityAt(m_globalPose0.p);
        if (m_body1)
            vel += m_body1->getVelocityAt(m_globalPose1.p);

        vel *= std::min(1.0f, m_posDamping * h);

        XPBDSolver::applyBodyPairCorrection(
            m_body0.get(), 
            m_body1.get(), 
            vel,
            0.0f,
            h,
            m_globalPose0.p, 
            m_globalPose1.p, 
            true
        );
    }

}
