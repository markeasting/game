#pragma once

#include "geom/Geometry.h"

class BoxGeometry : public Geometry {
public:

    BoxGeometry(float size = 1.0f) {
        this->generate(size, size, size);
    }
    
    BoxGeometry(float width, float height, float depth) {
        this->generate(width, height, depth);
    }
    

private:
    void generate(float width, float height, float depth) {
        
        const float x_dist = 0.5f * width;
        const float y_dist = 0.5f * height;
        const float z_dist = 0.5f * depth;

        std::vector<Vertex> vertices {
            Vertex(glm::vec3(-x_dist, -y_dist, -z_dist)), // Front
            Vertex(glm::vec3( x_dist, -y_dist, -z_dist)), // Front
            Vertex(glm::vec3( x_dist,  y_dist, -z_dist)), // Front
            Vertex(glm::vec3(-x_dist,  y_dist, -z_dist)), // Front
            Vertex(glm::vec3( x_dist, -y_dist,  z_dist)), // Back
            Vertex(glm::vec3( x_dist,  y_dist,  z_dist)), // Back
            Vertex(glm::vec3(-x_dist,  y_dist,  z_dist)), // Back
            Vertex(glm::vec3(-x_dist, -y_dist,  z_dist)), // Back
        }; 

        std::vector<unsigned int> indices = {
            2, 1, 0, // Front
            3, 2, 0, // Front
            1, 5, 4, // Right
            1, 2, 5, // Right
            4, 5, 6, // Back
            4, 6, 7, // Back
            7, 6, 3, // Left
            7, 3, 0, // Left
            0, 1, 7, // Bottom
            1, 4, 7, // Bottom
            2, 3, 6, // Top
            2, 6, 5, // Top
        };

        this->setData(
            ref<VertexBuffer>(vertices), 
            ref<IndexBuffer>(indices)
        );
    }
};
