#pragma once

#include "common.h"

#include "util/rotateVec2.h"

class Plane {
public:
    
	vec2 size;

    vec3 normal = { 0, 1.0f, 0 };
	vec3 center = { 0, 0, 0};

    float constant = 0.0f;

	Plane() = default;
    Plane(vec3 normal, float constant, vec2 size = { FLT_MAX, FLT_MAX }, vec3 center = { 0, 0, 0 })
        : normal(normal), normalReference(normal), constant(constant), size(size), center(center) {};

	void transform(const vec3& position, const quat& rotation) {
		this->center = position;
		this->normal = rotation * this->normalReference;
		this->constant = -glm::dot(position, this->normal);
		
		this->halfExtentsWorldSpace = 0.5f * rotateVec2(this->size, glm::mat3_cast(rotation));

		this->xAxis = normalize(cross(rotation * vec3(1, 0, 0), this->normal));
		this->yAxis = normalize(cross(this->normal, xAxis));
	}

	float distanceToPoint(const vec3& point) const {
		return glm::dot(normal, point) + constant;
	}

	vec3 projectPoint(const vec3& point) const {
		return point + (normal * -this->distanceToPoint(point));
	}

	bool containsPoint(const vec3& point) const {
		const vec3 projected = this->projectPoint(point - this->center);

		vec2 localPoint = vec2(
			glm::dot(projected, xAxis),
			glm::dot(projected, yAxis)
		);

		return (
			localPoint.x >= -halfExtentsWorldSpace.x &&
			localPoint.x <= halfExtentsWorldSpace.x &&
			localPoint.y >= -halfExtentsWorldSpace.y && 
			localPoint.y <= halfExtentsWorldSpace.y
		);
	}

    // float distanceToSphere( sphere ) {
	// 	return distanceToPoint( sphere.center ) - sphere.radius;
	// }

private:
	vec3 normalReference = vec3(0, 1.0f, 0);
	vec2 halfExtentsWorldSpace;

	vec3 xAxis;
	vec3 yAxis;
};
