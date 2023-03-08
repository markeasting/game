
#include "common_includes.h"
#include "gfx/Material.h"

Material::Material(const std::string& shaderBasePath, std::vector<Ref<IUniform>> uniforms) {
    m_shader = ref<Shader>(shaderBasePath);

    assert(m_shader != nullptr);
    // assert(m_shader->m_program != 0);

    for (int i = 0; i < uniforms.size(); i++)
        assignUniform(uniforms[i]);
}

Material::Material(const std::string& vert, const std::string& frag, std::vector<Ref<IUniform>> uniforms) {
    m_shader = ref<Shader>(vert, frag);

    assert(m_shader != nullptr);
    // assert(m_shader->m_program != 0);

    for (int i = 0; i < uniforms.size(); i++)
        assignUniform(uniforms[i]);
}

void Material::assignUniform(Ref<IUniform> uniform) {
    if(uniforms.find(uniform->m_name) == uniforms.end()) {
        uniform->m_location = m_shader->getUniformLocation(uniform->m_name);
        uniforms[uniform->m_name] = uniform;
    }
}

void Material::assignTexture(Ref<Texture> texture) {
    textures.push_back(texture);
}

void Material::assignTexture(const char* source) {

    Ref<Texture> texture = ref<Texture>();
    texture->load(source);

    textures.push_back(texture);
}

void Material::bind() const {
    m_shader->bind();

    for (auto texture : textures) {
        texture->bind();
    }

    for (const auto &nameUniformPair : uniforms) {
        nameUniformPair.second->bind();
    }
}
