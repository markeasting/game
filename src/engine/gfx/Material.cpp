
#include "engine/common.h"
#include "engine/gfx/Material.h"

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
    if (uniforms.find(uniform->m_name) == uniforms.end()) {
        uniform->m_location = m_shader->getUniformLocation(uniform->m_name);
        uniforms[uniform->m_name] = uniform;
    }
}

void Material::assignTexture(Ref<Texture> texture, const std::string& uniform) {
    this->setUniform(uniform, (int) textures.size());

    /** @TODO this doesn't seem to work, most textures have alpha data but aren't actually transparent. */
    /** Maybe just set `transparent` manually. */
    if (texture->m_format == GL_RGBA)
        transparent = true;

    textures.push_back(texture);
}

void Material::assignTexture(const char* source, const std::string& uniform) {

    Ref<Texture> texture = ref<Texture>();
    texture->load(source);

    this->assignTexture(texture, uniform);
}

void Material::bind() const {

    glUseProgram(m_shader->m_program);

    for (int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        // glBindTexture(GL_TEXTURE_2D, textures[i]->m_texture);
        textures[i]->bind();
    }

    for (const auto &[key, uniform] : uniforms) {
        uniform->bind();
    }
}

// void Material::bind() const {
//     m_shader->bind();

//     for (int i = 0; i < textures.size(); i++) {
//         glActiveTexture(GL_TEXTURE0 + i);
//         textures[i]->bind();
//     }

//     for (const auto &[key, uniform] : uniforms) {
//         uniform->bind();
//     }
// }
