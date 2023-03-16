#include "scene/Scene.h"

Scene::Scene() {
    
}

void Scene::add(Ref<Object3D> object) {
    m_children.push_back(object);
}


void Scene::add(Ref<Mesh> mesh) {
    
    if (mesh->m_material == nullptr)
        mesh->setMaterial(m_defaultMaterial);

    if (mesh->m_material->m_shader->m_program == 0) {
        Log("Shader failed to compile, using default shader");
        mesh->setMaterial(m_defaultMaterial);
    }

    m_meshes.push_back(mesh);
    m_children.push_back(mesh);
}
