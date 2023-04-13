
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
    for (auto const& f : objModel.faces) {
        for (auto const& i : f.second) {
            indices.push_back(i);
        }
    }

    for (size_t i = 0; i < objModel.vertex.size(); i += 3) {
    
        float vx = objModel.vertex[i];
        float vy = objModel.vertex[i + 1];
        float vz = objModel.vertex[i + 2];

        float nx = objModel.normal.size() ? objModel.normal[i] : 0.0f;
        float ny = objModel.normal.size() ? objModel.normal[i + 1] : 0.0f;
        float nz = objModel.normal.size() ? objModel.normal[i + 2] : 0.0f;

        float u = objModel.texCoord.size() ? objModel.texCoord[i] : 0.0f;
        float v = objModel.texCoord.size() ? objModel.texCoord[i + 1] : 0.0f;

        vertices.push_back(Vertex(
            vec3(vx, vy, vz),
            vec3(nx, ny, nz)
        ));
    }

    /* Untested */
    for (size_t i = 0; i < vertices.size(); i ++) {
        auto u = objModel.vertex[i * 2];
        auto v = objModel.vertex[i * 2 + 1];
        vertices[i].uv = vec2(u, v);
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
