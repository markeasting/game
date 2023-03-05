
#include "common_includes.h"
#include "gfx/Vertex.h"
#include "gfx/VertexBuffer.h"

VertexBuffer::VertexBuffer() {}

VertexBuffer::VertexBuffer(std::vector<Vertex> vertices, int mode) {
    setVertices(vertices, mode);
};

void VertexBuffer::setVertices(std::vector<Vertex> vertices, int mode) {

    m_vertices = vertices;
    m_count = vertices.size();
    m_size = m_count * sizeof(Vertex);
    
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_size, &vertices[0], mode);

    Vertex::setAttribPointers();
}

void VertexBuffer::bind() const {
    glBindVertexArray(m_vao); 
    // glBindBuffer(GL_ARRAY_BUFFER, m_buffer); // Not required?
}

void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

unsigned int VertexBuffer::getCount() const {
    return m_count;
}

unsigned int VertexBuffer::getSize() const {
    return m_size;
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_buffer);
    glDeleteBuffers(1, &m_vao);
}
