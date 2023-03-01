
#include "common.h"
#include "gfx/Material.h"

Material::Material() {}

Material::Material(const std::string& shaderBasePath, std::vector<IUniform*> uniforms) {
    this->shader = new Shader(shaderBasePath);
    assert(this->shader != nullptr);
    assert(this->shader->m_program != 0);

    for (int i = 0; i < uniforms.size(); i++)
        this->assignUniform(uniforms[i]);
}

void Material::setShader(Shader* shader) {
    this->shader = shader;
}

void Material::assignUniform(IUniform* uniform) {
    if(this->uniforms.find(uniform->name) == this->uniforms.end()) {
        uniform->location = this->shader->getUniformLocation(uniform->name);
        uniforms[uniform->name] = uniform;
    }
}

void Material::bind() {
    this->shader->bind();
    for (const auto &nameUniformPair : this->uniforms) {
        nameUniformPair.second->bind();
    }
}
