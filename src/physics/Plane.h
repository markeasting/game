#pragma once

#include "common.h"

class Plane {
public:
    
    vec3 normal = { 0, 1.0f, 0 };
    float constant = 0.0f;

	Plane() = default;
    Plane(vec3 normal, float constant)
        : normal(normal), constant(constant) {};

	void normalize() {

		const float invLen = 1.0f / normal.length();
		
        normal *= invLen;
		constant *= invLen;

	}

	float distanceToPoint(vec3 point) const {

		return glm::dot(normal, point) + constant;

	}

    /* @TODO TEST */
	vec3 projectPoint(vec3 point) const {
		return point + (normal * -Plane::distanceToPoint(point));
	}

    // float distanceToSphere( sphere ) {
	// 	return distanceToPoint( sphere.center ) - sphere.radius;
	// }

};
