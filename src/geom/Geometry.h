#pragma once

#include "gfx/BufferObject.h"
#include "geom/PrimitiveMesh.h"

class Renderer;

class Geometry {
public:

    /* @TODO make these unique ptr? This class should 'own' the buffers */
    Geometry();
    Geometry(Ref<VertexBuffer> vbo, Ref<IndexBuffer> ibo = nullptr);
    Geometry(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices = {});
    Geometry(const PrimitiveMesh& primitiveMesh);

    // template<typename T>
    // void setAttribute(BufferObject<T> attribute) {
    //     auto buffer = std::dynamic_pointer_cast<BufferObject<T>>(m_attributes.at(name));
    //     buffer->set(value);
    // };

    void bind() const;
    void unbind() const;

    bool hasIndices() const { return m_indexBuffer != nullptr; }

private:
    friend Renderer;

    GLuint m_vao = 0;

    Ref<IndexBuffer> m_indexBuffer = nullptr;
    Ref<VertexBuffer> m_vertexBuffer = nullptr;

};
