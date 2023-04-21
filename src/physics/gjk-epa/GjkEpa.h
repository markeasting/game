#pragma once

#include "physics/Collider.h"
#include "physics/gjk-epa/Simplex.h"
#include "physics/gjk-epa/Support.h"

namespace GjkEpa {

    struct Contact
    {
        bool exists = false;

        vec3 normal;
        vec3 p1;
        vec3 p2;
        float d;

        Contact(
            vec3 normal = vec3(0),
            vec3 p1 = vec3(0),
            vec3 p2 = vec3(0),
            float d = 0.0f,
            bool exists = false
        ): normal(normal), p1(p1), p2(p2), d(d), exists(exists) {}
    };

    inline bool sameDirection(vec3 direction, vec3 ao) {
        return glm::dot(direction, ao) > 0;
    }

    Simplex GJK(
        Collider* colliderA,
        Collider* colliderB
    );

    /**
     * Returns the vertex on the Minkowski difference
     */
    Support support(
        Collider* colliderA,
        Collider* colliderB,
        vec3 direction
    );

    bool nextSimplex(
        Simplex &simplex,
        vec3 &direction
    );

    bool Line(
        Simplex &simplex,
        vec3 &direction
    );

    bool Triangle(
        Simplex &simplex,
        vec3 &direction
    );

    bool Tetrahedron(
        Simplex &simplex,
        vec3 &direction
    );

    /**
     * EPA algorithm
     * 
     * https://github.com/IainWinter/IwEngine/blob/master/IwEngine/src/physics/impl/GJK.cpp
     */
    Contact EPA(
        const Simplex& simplex,
        Collider* colliderA,
        Collider* colliderB
    );

    vec3 computeBarycentricCoordinates(
        const vec3& P, 
        const std::vector<Support>& polygon
    );

    std::tuple<std::vector<vec4>, size_t, std::vector<Support>> getFaceNormals(
		const std::vector<Support>& polytope,
		const std::vector<size_t>& faces
    );

    void addIfUniqueEdge(
        std::vector<std::pair<size_t, size_t>>& edges,
        const std::vector<size_t>& faces,
        size_t a,
        size_t b
    );
};
