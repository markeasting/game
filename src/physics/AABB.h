#pragma once

#include "common.h"
#include "physics/Plane.h"

class AABB {
public:

    vec3 min = vec3(-std::numeric_limits<float>::infinity());
    vec3 max = vec3(std::numeric_limits<float>::infinity());

    AABB() = default;

	inline void set(vec3 _min, vec3 _max) {
		min = _min;
		max = _max;
	}

    inline void expandByScalar(float scalar) {
        min -= scalar;
        max += scalar;
    }

    inline bool containsPoint(vec3 point) const {
        return point.x < min.x || point.x > max.x ||
            point.y < min.y || point.y > max.y ||
            point.z < min.z || point.z > max.z ? false : true;
    }

    inline bool intersects(AABB other) const {
        return other.max.x < min.x || other.min.x > max.x ||
            other.max.y < min.y || other.min.y > max.y ||
            other.max.z < min.z || other.min.z > max.z ? false : true;
    }

    inline bool intersectsPlane(Plane plane) const {
        float _min, _max;

		if ( plane.normal.x > 0 ) {
			_min = plane.normal.x * min.x;
			_max = plane.normal.x * max.x;
		} else {
			_min = plane.normal.x * max.x;
			_max = plane.normal.x * min.x;
		}

		if ( plane.normal.y > 0 ) {
			_min += plane.normal.y * min.y;
			_max += plane.normal.y * max.y;
		} else {
			_min += plane.normal.y * max.y;
			_max += plane.normal.y * min.y;
		}

		if ( plane.normal.z > 0 ) {
			_min += plane.normal.z * min.z;
			_max += plane.normal.z * max.z;
		} else {
			_min += plane.normal.z * max.z;
			_max += plane.normal.z * min.z;
		}

		return ( _min <= - plane.constant && _max >= - plane.constant );
    }

};
