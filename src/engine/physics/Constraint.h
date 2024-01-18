#pragma once

#include "engine/common.h"
#include "engine/physics/RigidBody.h"

class Constraint {
public:
    
    Ref<RigidBody> m_body0 = nullptr;
    Ref<RigidBody> m_body1 = nullptr;

    float m_compliance = 0.0; // [meters / Newton] -- inverse of 'stiffness' (N/m)
    float m_lambda = 0.0; 
    vec3 m_normal;

    float m_posDamping = 0.0;
    float m_rotDamping = 0.0;

    Pose m_localPose0;
    Pose m_localPose1;
    Pose m_globalPose0;
    Pose m_globalPose1;

    Constraint() = default;
    virtual ~Constraint() = default;

    void setBodies(Ref<RigidBody> body0, Ref<RigidBody> body1, Pose localPose0, Pose localPose1);
    void setBodies(Ref<RigidBody> body0, Ref<RigidBody> body1, vec3 localPose0 = vec3(0), vec3 localPose1 = vec3(0));

    // m/N
    void setCompliance(float compliance);
    
    // N/m
    void setStiffness(float stiffness);

    void setDamping(float posDamping, float rotDamping);

    vec3 getForce(float h) const;

    void updateGlobalPoses();

    virtual void solvePos(float h);
    void solveVel(float h);

};
