#pragma once

#include "geom/Geometry.h"

class PlaneGeometry : public Geometry {
public:

    PlaneGeometry(float size = 1.0f) {
        this->generate(size, size);
    };
    PlaneGeometry(float width, float height) {
        this->generate(width, height);
    };

private:
    void generate(float width, float height) {

        float x = 0.5f * width;
        float y = 0.5f * height;
        
        std::vector<Vertex> vertices {
            Vertex(glm::vec3(  x, -y, 0 ), glm::vec3( 0, 0, 1 ), glm::vec2( 1, 0 )),
            Vertex(glm::vec3(  x,  y, 0 ), glm::vec3( 0, 0, 1 ), glm::vec2( 1, 1 )),
            Vertex(glm::vec3( -x, -y, 0 ), glm::vec3( 0, 0, 1 ), glm::vec2( 0, 0 )),
            Vertex(glm::vec3( -x,  y, 0 ), glm::vec3( 0, 0, 1 ), glm::vec2( 0, 1 )),
        };

        std::vector<unsigned int> indices = {
            0, 1, 2,
            2, 1, 3,
        };

        this->setData(
            ref<VertexBuffer>(vertices), 
            ref<IndexBuffer>(indices)
        );
    };
};
