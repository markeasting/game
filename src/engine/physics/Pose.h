#pragma once

#include "engine/common/glm.h"

class Pose {
public:

    glm::vec3 p = glm::vec3(0);
    glm::quat q = glm::quat(1.0f, 0, 0, 0);		

    Pose() = default; 
    Pose(glm::vec3 p, glm::quat q = glm::quat(1.0f, 0, 0, 0)); 

    void copy(Pose pose);

    Pose clone();
    
    void translate(glm::vec3& v) const;
    void invTranslate(glm::vec3& v) const;

    void rotate(glm::vec3& v) const;
    void invRotate(glm::vec3& v) const;

    void transform(glm::vec3& v) const;
    void invTransform(glm::vec3& v) const;

    void transformPose(Pose& pose) const;  

};
