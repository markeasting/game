
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
    assert(m_vao != 0);

    m_vertexBuffer = vbo;
    m_indexBuffer = ibo;

    Vertex::setAttribPointers();

    /* Not necesary but a nice safe guard */
    this->unbind();
}

Geometry::Geometry(const PrimitiveMesh& primitiveMesh) 
: Geometry() /* Delegate VAO creation to base constructor */
{
    assert(m_vao != 0);

    m_vertexBuffer = ref<VertexBuffer>(primitiveMesh.vertices);
    m_indexBuffer = ref<IndexBuffer>(primitiveMesh.indices);

    Vertex::setAttribPointers();

    /* Not necesary but a nice safe guard */
    this->unbind();
}

Geometry::Geometry(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) 
: Geometry(PrimitiveMesh(vertices, indices)) /* Delegate to PrimitiveMesh constructor */
{}

void Geometry::bind() const {
    assert(m_vao != 0);
    glBindVertexArray(m_vao); 
}

void Geometry::unbind() const {
    glBindVertexArray(0); 
}
