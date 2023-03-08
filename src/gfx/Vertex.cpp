
#include "common_includes.h"
#include "gfx/Vertex.h"

Vertex::Vertex() {}

// Vertex::Vertex(glm::vec3 position)
//     : position(position)
// {}

// Vertex::Vertex(glm::vec3 position, glm::vec3 normal)
//     : position(position), normal(normal)
// {}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv)
    : position(position), normal(normal), uv(uv)
{}

Vertex::Vertex(float x, float y, float z)
    : position { x, y, z }
{}

void Vertex::setAttribPointers() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, uv));
    // glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    // glEnableVertexAttribArray(3);
}
