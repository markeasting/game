
#include "engine/gfx/Mesh.h"

Mesh::Mesh() : 
    m_geometry(nullptr),
    m_material(nullptr) 
{}

Mesh::Mesh(const Geometry& geometry, const Material& material) :
    m_geometry(ref<Geometry>(geometry)),
    m_material(ref<Material>(material))
{}

Mesh::Mesh(const Geometry& geometry) :
    m_geometry(ref<Geometry>(geometry))
{}

Mesh::Mesh(Ref<Geometry> geometry, const Material& material) :
    m_geometry(geometry),
    m_material(ref<Material>(material))
{
    assert(m_geometry != nullptr);
}

void Mesh::bind() const {
    assert(m_geometry != nullptr);
    assert(m_material != nullptr);

    m_geometry->bind();
    m_material->bind();
}

void Mesh::unbind() const {
    assert(m_geometry != nullptr);
    
    m_geometry->unbind();
}

void Mesh::setMaterial(const Material& material) {
    m_material = std::make_shared<Material>(material);
}
