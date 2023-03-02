
#include "common.h"
#include "gfx/Material.h"

Material::Material(const std::string& shaderBasePath, std::vector<IUniform*> uniforms) {
    this->m_shader = std::make_shared<Shader>(shaderBasePath);

    // assert(this->m_shader != nullptr);
    assert(this->m_shader->m_program != 0);

    for (int i = 0; i < uniforms.size(); i++)
        this->assignUniform(uniforms[i]);
}

void Material::assignUniform(IUniform* uniform) {
    if(this->uniforms.find(uniform->name) == this->uniforms.end()) {
        uniform->location = this->m_shader->getUniformLocation(uniform->name);
        uniforms[uniform->name] = uniform;
    }
}

void Material::bind() const {
    this->m_shader->bind();

    for (const auto &nameUniformPair : this->uniforms) {
        nameUniformPair.second->bind();
    }
}
