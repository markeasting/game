#pragma once

#include <cmath>

namespace Anim {
    inline void lerp(float& value, float target, float factor = 0.98) {
        value = std::lerp(value, target, factor);
    }
}
