#include "scene/Layer.h"

Layer::Layer() {}

void Layer::add(Ref<Object3D> object) {
    assert(object != nullptr);

    m_children.push_back(object);
}

void Layer::add(Ref<Mesh> mesh) {

    assert(mesh != nullptr);
    
    if (mesh->m_material == nullptr)
        mesh->setMaterial(m_defaultMaterial);

    if (mesh->m_material->m_shader->m_program == 0) {
        Log("Shader failed to compile, using default shader");
        mesh->setMaterial(m_defaultMaterial);
    }

    m_meshes.push_back(mesh);
    m_children.push_back(mesh);
}

// void Layer::init() {}
// void Layer::destroy() {}

// void Layer::onActivate() {}
// void Layer::onDeactivate() {}
// void Layer::update(float time, float dt) {}
