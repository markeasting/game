#pragma once

#include "geom/Geometry.h"

class SphereGeometry : public Geometry {
public:

    SphereGeometry(float size = 1.0f, unsigned int segments = 20) {
        this->generate(size, segments);
    };

private:
    void generate(float size = 1.0f, unsigned int segments = 20) {

        // http://www.songho.ca/opengl/gl_sphere.html
        float radius = size/2;
        int sectorCount = segments;
        int stackCount = segments;
        
        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
        float s, t;                                     // texCoord

        float sectorStep = 2 * M_PI / sectorCount;
        float stackStep = M_PI / stackCount;
        float sectorAngle, stackAngle;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for(int i = 0; i <= stackCount; ++i)
        {
            stackAngle = M_PI / 2 - i * stackStep;      // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for(int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

                // normalized vertex normal
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;

                // // vertex tex coord between [0, 1]
                // s = (float)j / sectorCount;
                // t = (float)i / stackCount;

                vertices.push_back(Vertex(
                    glm::vec3(x, y, z),
                    glm::vec3(nx, ny, nz)
                ));
                
            }
        }

        // indices
        //  k1--k1+1
        //  |  / |
        //  | /  |
        //  k2--k2+1
        unsigned int k1, k2;
        for(int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding 1st and last stacks
                if(i != 0)
                {
                    // addIndices(k1, k2, k1+1);   // k1---k2---k1+1
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1+1);
                }

                if(i != (stackCount-1))
                {
                    // addIndices(k1+1, k2, k2+1); // k1+1---k2---k2+1
                    indices.push_back(k1+1);
                    indices.push_back(k2);
                    indices.push_back(k2+1);
                }
            }
        }

        this->setData(
            ref<VertexBuffer>(vertices), 
            ref<IndexBuffer>(indices)
        );
    };
};
