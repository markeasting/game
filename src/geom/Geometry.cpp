
#include "common.h"
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

Geometry::Geometry(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) 
    : Geometry() /* Delegate VAO creation to base constructor */
{
    this->setData(
        ref<VertexBuffer>(vertices), 
        ref<IndexBuffer>(indices)
    );
}

Geometry::Geometry(const obj::Model &objModel) 
    : Geometry() /* Delegate VAO creation to base constructor */
{

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // @TODO load vertex attribs into separate buffers (non-interleaved)
    for (auto f : objModel.faces) {
        for (auto i : f.second) {
            indices.push_back(i);
        }
    }

    for (size_t i = 0; i < objModel.vertex.size(); i += 3) {
    
        auto vx = objModel.vertex[i];
        auto vy = objModel.vertex[i + 1];
        auto vz = objModel.vertex[i + 2];

        auto nx = objModel.normal[i];
        auto ny = objModel.normal[i + 1];
        auto nz = objModel.normal[i + 2];

        auto u = objModel.texCoord[i];
        auto v = objModel.texCoord[i + 1];

        vertices.push_back(Vertex(
            glm::vec3(vx, vy, vz),
            glm::vec3(nx, ny, nz)
        ));
    }

    /* Untested */
    for (size_t i = 0; i < vertices.size(); i ++) {
        auto u = objModel.vertex[i * 2];
        auto v = objModel.vertex[i * 2 + 1];
        vertices[i].uv = glm::vec2(u, v);
    }

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
