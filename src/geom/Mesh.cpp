#include "geom/Mesh.h"

Mesh::Mesh(const PrimitiveMesh& primitiveMesh) 
    : vertexBuffer(primitiveMesh.vertices), indexBuffer(primitiveMesh.indices), material(nullptr) {}

void Mesh::bind() const {
    this->vertexBuffer.bind();
    this->indexBuffer.bind();
    this->material->bind();
}

void Mesh::unbind() const {
    this->vertexBuffer.unbind();
    this->indexBuffer.unbind();
}

void Mesh::setMaterial(Material* material) {
    this->material = material;
}
