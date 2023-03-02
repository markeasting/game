#include "geom/Mesh.h"

Mesh::Mesh() : 
    m_vertexBuffer(std::make_shared<VertexBuffer>()), 
    m_indexBuffer(std::make_shared<IndexBuffer>()), 
    m_material(nullptr) 
{}

Mesh::Mesh(const PrimitiveMesh& primitiveMesh) : 
    m_vertexBuffer(std::make_shared<VertexBuffer>(primitiveMesh.vertices)), 
    m_indexBuffer(std::make_shared<IndexBuffer>(primitiveMesh.indices)), 
    m_material(nullptr) 
{}

Mesh::Mesh(const PrimitiveMesh& primitiveMesh, const Material& material) : 
    m_vertexBuffer(std::make_shared<VertexBuffer>(primitiveMesh.vertices)), 
    m_indexBuffer(std::make_shared<IndexBuffer>(primitiveMesh.indices)), 
    m_material(std::make_shared<Material>(material))
{}

void Mesh::bind() const {
    m_vertexBuffer->bind();
    m_indexBuffer->bind();
    m_material->bind();
}

void Mesh::unbind() const {
    m_vertexBuffer->unbind();
    m_indexBuffer->unbind();
}

void Mesh::setMaterial(const Material& material) {
    m_material = std::make_shared<Material>(material);
}
