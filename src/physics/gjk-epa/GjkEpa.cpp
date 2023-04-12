
#include "physics/gjk-epa/GjkEpa.h"

namespace GjkEpa {

    Simplex GJK(
        Collider* colliderA,
        Collider* colliderB
    ) {

        assert(colliderA != nullptr);
        assert(colliderB != nullptr);

        /**
         * We need at least one vertex to start, so we’ll manually add it.
         * The search direction for the first vertex doesn’t matter, but you
         * may get less iterations with a smarter choice.
         */
        Support support = GjkEpa::support(colliderA, colliderB, {0, 1, 0});

        /* Simplex is an array of points, max count is 4 */
        Simplex simplex;
        simplex.push_front(support);

        /* New direction is towards the origin */
        vec3 direction = -support.point;

        while (true) {

            /**
             * In a loop, we’ll add another point.
             *
             * The exit condition is that this new point is not in front
             * of the search direction. This would exit if the direction
             * finds a vertex that was already the furthest one along it.
             */

            auto support = GjkEpa::support(colliderA, colliderB, direction);

            if (glm::dot(support.point, direction) <= 0)
                return simplex; /* No collision */

            simplex.push_front(support);

            /**
             * Now that we have a line, we’ll feed it into a function that
             * updates the simplex and search direction.
             * It’ll return true or false to signify a collision.
             */
            if (GjkEpa::nextSimplex(simplex, direction))
            {
                simplex.containsOrigin = true;
                return simplex;
            }
        }
    }

    /**
     * Returns the vertex on the Minkowski difference
     */
    Support support(
        Collider* colliderA,
        Collider* colliderB,
        vec3 direction
    ) {
        vec3 witnessA = colliderA->findFurthestPoint(direction);
        vec3 witnessB = colliderB->findFurthestPoint(-direction);

        return { witnessA, witnessB };
    }

    bool nextSimplex(
        Simplex& simplex,
        vec3 &direction)
    {

        switch (simplex.size())
        {
        case 2:
            return GjkEpa::Line(simplex, direction);
        case 3:
            return GjkEpa::Triangle(simplex, direction);
        case 4:
            return GjkEpa::Tetrahedron(simplex, direction);
        }

        return false;
    }

    bool Line(
        Simplex& simplex,
        vec3 &direction
    ) {
        const auto &a = simplex[0];
        const auto &b = simplex[1];

        vec3 ab = b.point - a.point;
        vec3 ao = -a.point;

        if (GjkEpa::sameDirection(ab, ao))
        {
            direction = glm::cross(glm::cross(ab, ao), ab);
        }
        else
        {
            simplex.assign({a});
            direction = ao;
        }

        return false;
    }

    bool Triangle(
        Simplex& simplex,
        vec3 &direction
    ) {
        const auto &a = simplex[0];
        const auto &b = simplex[1];
        const auto &c = simplex[2];

        vec3 ab = b.point - a.point;
        vec3 ac = c.point - a.point;
        vec3 ao = -a.point;

        vec3 abc = glm::cross(ab, ac);

        if (GjkEpa::sameDirection(glm::cross(abc, ac), ao)) {
            if (GjkEpa::sameDirection(ac, ao)) {
                simplex.assign({a, c});
                direction = glm::cross(glm::cross(ac, ao), ac);
            } else {
                simplex.assign({a, b});
                return Line(simplex, direction);
            }
        } else {
            if (GjkEpa::sameDirection(glm::cross(ab, abc), ao))
            {
                simplex.assign({a, b});
                return Line(simplex, direction);
            } else {
                if (GjkEpa::sameDirection(abc, ao)) {
                    direction = abc;
                } else {
                    simplex.assign({a, c, b});
                    direction = -abc;
                }
            }
        }

        return false;
    }

    bool Tetrahedron(
        Simplex& simplex,
        vec3 &direction
    ) {
        const auto &a = simplex[0];
        const auto &b = simplex[1];
        const auto &c = simplex[2];
        const auto &d = simplex[3];

        vec3 ab = b.point - a.point;
        vec3 ac = c.point - a.point;
        vec3 ad = d.point - a.point;
        vec3 ao = -a.point;

        vec3 abc = glm::cross(ab, ac);
        vec3 acd = glm::cross(ac, ad);
        vec3 adb = glm::cross(ad, ab);

        if (GjkEpa::sameDirection(abc, ao)) {
            simplex.assign({a, b, c});
            return Triangle(simplex, direction);
        }

        if (GjkEpa::sameDirection(acd, ao)) {
            simplex.assign({a, c, d});
            return Triangle(simplex, direction);
        }

        if (GjkEpa::sameDirection(adb, ao)) {
            simplex.assign({a, d, b});
            return Triangle(simplex, direction);
        }

        return true;
    }


    /**
     * EPA algorithm
     * 
     * https://github.com/IainWinter/IwEngine/blob/master/IwEngine/src/physics/impl/GJK.cpp
     */
    Contact EPA(
        const Simplex& simplex,
        Collider* colliderA,
        Collider* colliderB
    ) {

        assert(colliderA != nullptr);
        assert(colliderB != nullptr);

        // std::vector<Support> polytope(simplex.begin(), simplex.end());
        std::vector<Support> polytope;

        for (size_t i = 0; i < simplex.size(); i++)
            polytope.push_back(simplex.m_points[i]);

        std::vector<size_t> faces = {
            0, 1, 2,
            0, 3, 1,
            0, 2, 3,
            1, 3, 2};

        auto [
            normals,
            minFace,
            minPolygon
        ] = GjkEpa::getFaceNormals(polytope, faces);

        vec3 minNormal;
        float minDistance = FLT_MAX;

        size_t iterations = 0;
        while (minDistance == FLT_MAX) {
            minNormal.x = normals[minFace].x;
            minNormal.y = normals[minFace].y; 
            minNormal.z = normals[minFace].z;
            minDistance = normals[minFace].w;
            
            if (iterations++ > 16) {
                // Log("Too many EPA iterations");
                break;
            }

            vec3 witnessA = colliderA->findFurthestPoint(minNormal);
            vec3 witnessB = colliderB->findFurthestPoint(-minNormal);

            Support support = { witnessA, witnessB };

            float sDistance = glm::dot(minNormal, support.point);

            if (std::abs(sDistance - minDistance) > 0.001f) {

                minDistance = FLT_MAX;

				std::vector<std::pair<size_t, size_t>> uniqueEdges;

				for (size_t i = 0; i < normals.size(); i++) {
                    vec3 n = { normals[i].x, normals[i].y, normals[i].z };

                    if (GjkEpa::sameDirection(n, support.point)) {
                        size_t f = i * 3;

                        GjkEpa::addIfUniqueEdge(uniqueEdges, faces, f + 0, f + 1);
                        GjkEpa::addIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
                        GjkEpa::addIfUniqueEdge(uniqueEdges, faces, f + 2, f + 0);

						faces[f + 2] = faces.back(); faces.pop_back();
						faces[f + 1] = faces.back(); faces.pop_back();
						faces[f    ] = faces.back(); faces.pop_back();

						normals[i] = normals.back(); normals.pop_back();

                        i--;
                    }
                }

				if (uniqueEdges.size() == 0) {
					break;
				}

				std::vector<size_t> newFaces;
				for (auto [edge1, edge2] : uniqueEdges) {
					newFaces.push_back(edge1);
					newFaces.push_back(edge2);
					newFaces.push_back(polytope.size());
				}

				polytope.push_back(support);

                auto [
                    newNormals,
                    newMinFace,
                    newPolygon
                ] = GjkEpa::getFaceNormals(polytope, newFaces);

				float newMinDistance = FLT_MAX;
				for (size_t i = 0; i < normals.size(); i++) {
					if (normals[i].w < newMinDistance) {
						newMinDistance = normals[i].w;
						minFace = i;
					}
				}

                if (newNormals[newMinFace].w < newMinDistance) {
                    minFace = newMinFace + normals.size();
                    minPolygon = newPolygon;
                }

				faces  .insert(faces  .end(), newFaces  .begin(), newFaces  .end());
				normals.insert(normals.end(), newNormals.begin(), newNormals.end());
            }

        }

        if (minDistance == FLT_MAX) {
            // @TODO make a neater 'empty' return statement
			return {
                vec3(0),
                vec3(0),
                vec3(0),
                0.0f,
                false
            };
		}

        /* Contact point (v) */
        vec3 contactPoint = minNormal * minDistance;

        /* Triangle on the Minkowski boundary that contains v */
        vec3 barycentric = GjkEpa::computeBarycentricCoordinates(contactPoint, minPolygon);

        /* Find contact point on the original shapes */
        vec3 a = minPolygon[0].witnessA * barycentric.x;
        vec3 b = minPolygon[1].witnessA * barycentric.y;
        vec3 c = minPolygon[2].witnessA * barycentric.z;
        vec3 p1 = a + b + c;

        a = minPolygon[0].witnessB * barycentric.x;
        b = minPolygon[1].witnessB * barycentric.y;
        c = minPolygon[2].witnessB * barycentric.z;
        vec3 p2 = a + b + c;

		return {
            minNormal,
            p1,
            p2,
            minDistance,
            true
        };

    }

    vec3 computeBarycentricCoordinates(
        const vec3& P, 
        const std::vector<Support>& polygon
    ) {

        const auto A = polygon[0].point;
        const auto B = polygon[1].point;
        const auto C = polygon[2].point;

        const auto v0 = B - A;
        const auto v1 = C - A;
        const auto v2 = P - A;
        
        float dot00 = glm::dot(v0, v0);
        float dot01 = glm::dot(v0, v1);
        float dot02 = glm::dot(v0, v2);
        float dot11 = glm::dot(v1, v1);
        float dot12 = glm::dot(v1, v2);
        
        float denom = dot00 * dot11 - dot01 * dot01;
        float v = (dot11 * dot02 - dot01 * dot12) / denom;
        float w = (dot00 * dot12 - dot01 * dot02) / denom;
        float u = 1.0 - v - w;
        
        return { u, v, w };
    }

    std::tuple<std::vector<vec4>, size_t, std::vector<Support>> getFaceNormals(
		const std::vector<Support>& polytope,
		const std::vector<size_t>& faces
    ) {
		std::vector<vec4> normals;
		size_t minTriangle = 0;
		float minDistance = FLT_MAX;

        std::vector<Support> polygon = { polytope[0], polytope[1], polytope[3] };
        
		for (size_t i = 0; i < faces.size(); i += 3) {
			Support a = polytope[faces[i    ]];
			Support b = polytope[faces[i + 1]];
			Support c = polytope[faces[i + 2]];

			vec3 normal = glm::normalize(glm::cross(b.point - a.point, c.point - a.point));
			float distance = glm::dot(normal, a.point);

			if (distance < 0) {
				normal   *= -1;
				distance *= -1;
			}

			normals.emplace_back(normal, distance);

			if (distance < minDistance) {
				minTriangle = i / 3;
				minDistance = distance;

                polygon[0] = a;
                polygon[1] = b;
                polygon[2] = c;
			}
		}

		return std::make_tuple(normals, minTriangle, polygon);
	}

    void addIfUniqueEdge(
        std::vector<std::pair<size_t, size_t>>& edges,
        const std::vector<size_t>& faces,
        size_t a,
        size_t b
    ) {

        auto reverse = std::find(              //      0--<--3
            edges.begin(),                     //     / \ B /   A: 2-0
            edges.end(),                       //    / A \ /    B: 0-2
            std::make_pair(faces[b], faces[a]) //   1-->--2
        );

        if (reverse != edges.end()) {
            edges.erase(reverse);
        } else {
            edges.emplace_back(faces[a], faces[b]);
        }
    }
}

