#pragma once

#include "geom/Geometry.h"

class TetrahedronGeometry : public Geometry {
public:

    TetrahedronGeometry(float size = 1.0f) {
        this->generate(size);
    };

private:
    void generate(float size) {

        float sqrt8over9 = 0.9428090415820633658 * size;
        float sqrt2over9 = 0.4714045207910316829 * size;
        float sqrt2over3 = 0.8164965809277260327 * size;
        float oneThird = 0.333333333333333333 * size;

        std::vector<Vertex> vertices {
            Vertex(vec3(0, -oneThird, sqrt8over9)),
            Vertex(vec3(sqrt2over3, -oneThird, -sqrt2over9)), 
            Vertex(vec3(-sqrt2over3, -oneThird, -sqrt2over9)),
            Vertex(vec3(0, size, 0)),
        };

        std::vector<unsigned int> indices = {
            2, 1, 0,
            1, 2, 3,
            0, 3, 2,
            1, 3, 0
        };

        this->setData(
            ref<VertexBuffer>(vertices), 
            ref<IndexBuffer>(indices)
        );
    };
};
