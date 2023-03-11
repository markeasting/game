#pragma once

#include "common.h"
#include "gfx/Vertex.h"

#include <vector>

class IBufferObject {
public:

    // int m_data;

    IBufferObject() = default;
    ~IBufferObject() {
        glDeleteBuffers(1, &m_buffer);
    }

    /* Binding will be performed by VAO */
    // virtual void bind() const {};

    unsigned int getCount() const {
        return m_count;
    }

    unsigned int getSize() const {
        return m_size;
    }

protected:

	GLuint m_buffer = 0;
    GLenum m_usage = GL_STATIC_DRAW;
	
    // @TODO maybe store individual element size as well
	unsigned int m_count = 0;
	unsigned int m_size = 0;
};

template <typename T>
class BufferObject : public IBufferObject {
public:

    std::vector<T> m_data = {};

    BufferObject() = default;

    /* Assumes a VAO is bound when calling this method. */
    virtual void set(const std::vector<T>& data) { 
        m_data = data;
    }
};

class IndexBuffer : public BufferObject<unsigned int> {
public:

    IndexBuffer(const std::vector<unsigned int>& data = {}) {
        this->set(data);
    }

    /* Assumes a VAO is bound when calling this method. */
    virtual void set(const std::vector<unsigned int>& data) { 
        m_data = data;
        m_count = data.size();
        m_size = m_count * sizeof(unsigned int);

        glGenBuffers(1, &m_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, &m_data[0], m_usage);
    }
};

class VertexBuffer : public BufferObject<Vertex> {
public:

    VertexBuffer(const std::vector<Vertex>& data = {}) {
        this->set(data);
    }

    /* Assumes a VAO is bound when calling this method. */
    virtual void set(const std::vector<Vertex>& data) {
        m_data = data;
        m_count = data.size();
        m_size = m_count * sizeof(Vertex);

        glGenBuffers(1, &m_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
        glBufferData(GL_ARRAY_BUFFER, m_size, &m_data[0], m_usage);
    }
};
