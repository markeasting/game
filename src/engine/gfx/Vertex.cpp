
#include "engine/gfx/Vertex.h"
#include "engine/common/gl.h"

Vertex::Vertex() {}

Vertex::Vertex(vec3 position, vec3 normal, vec2 uv)
    : position(position), normal(normal), uv(uv)
{}

Vertex::Vertex(float x, float y, float z)
    : position { x, y, z }
{}

void Vertex::setAttribPointers() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, uv));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}
