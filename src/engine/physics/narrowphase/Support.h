#pragma once

#include "engine/common/glm.h"

struct Support {

    vec3 point;
    vec3 witnessA;
    vec3 witnessB;

    Support() = default;

    Support(
        vec3 witnessA,
        vec3 witnessB
    ) : witnessA(witnessA), witnessB(witnessB) {
        point = witnessA - witnessB;
    }

};
