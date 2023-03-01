
#include "common.h"
#include "gfx/Vertex.h"
#include "gfx/IndexBuffer.h"

IndexBuffer::IndexBuffer()
{}

/**
 * @param indices
 * @param mode GL_STATIC_DRAW or GL_DYNAMIC_DRAW. See glBufferData() for more options.
 */
IndexBuffer::IndexBuffer(std::vector<unsigned int> indices, int mode) {
    this->setIndices(indices, mode);
};

void IndexBuffer::setIndices(std::vector<unsigned int> indices, int mode) {
    
    this->indices = indices;
    this->count = indices.size();
    this->size = this->count * sizeof(unsigned int);

    glGenBuffers(1, &this->ibuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->getSize(), &indices[0], mode);

}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibuffer);
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::getCount() const {
    return this->count;
}

unsigned int IndexBuffer::getSize() const {
    return this->size;
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &this->ibuffer);
}
