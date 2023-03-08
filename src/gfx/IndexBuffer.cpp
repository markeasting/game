
#include "common_includes.h"
#include "gfx/Vertex.h"
#include "gfx/IndexBuffer.h"

IndexBuffer::IndexBuffer()
{}

/**
 * @param indices
 * @param mode GL_STATIC_DRAW or GL_DYNAMIC_DRAW. See glBufferData() for more options.
 */
IndexBuffer::IndexBuffer(std::vector<unsigned int> indices, int mode) {
    setIndices(indices, mode);
};

void IndexBuffer::setIndices(std::vector<unsigned int> indices, int mode) {
    
    m_indices = indices;
    m_count = indices.size();
    m_size = m_count * sizeof(unsigned int);

    glGenBuffers(1, &m_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, &m_indices[0], mode);

}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::getCount() const {
    return m_count;
}

unsigned int IndexBuffer::getSize() const {
    return m_size;
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_buffer);
}
