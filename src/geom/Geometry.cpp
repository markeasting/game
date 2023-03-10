
#include "common_includes.h"
#include "geom/Geometry.h"

Geometry::Geometry() {

    /**
     * Vertex Array Object with interleaved layout
     * 
     * | XYZ PQR UV | XYZ PQR UV | XYZ PQR UV |
     */
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

}

Geometry::Geometry(Ref<VertexBuffer> vbo, Ref<IndexBuffer> ibo) 
: Geometry() /* Delegate VAO creation to base constructor */
{
    this->setData(vbo, ibo);
}

Geometry::Geometry(const PrimitiveMesh& primitiveMesh) 
: Geometry() /* Delegate VAO creation to base constructor */
{
    this->setData(
        ref<VertexBuffer>(primitiveMesh.vertices), 
        ref<IndexBuffer>(primitiveMesh.indices)
    );
}

Geometry::Geometry(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) 
: Geometry() /* Delegate VAO creation to base constructor */
{
    this->setData(
        ref<VertexBuffer>(vertices), 
        ref<IndexBuffer>(indices)
    );
}

void Geometry::setData(Ref<VertexBuffer> vbo, Ref<IndexBuffer> ibo) {
    assert(m_vao != 0);

    m_vertexBuffer = vbo;
    m_indexBuffer = ibo;

    Vertex::setAttribPointers();
}

void Geometry::bind() const {
    assert(m_vao != 0);
    glBindVertexArray(m_vao); 
}

void Geometry::unbind() const {
    glBindVertexArray(0); 
}
