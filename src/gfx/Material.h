#pragma once

#include "common.h"
#include "gfx/Shader.h"
#include "gfx/Uniforms.h"
#include "gfx/Texture.h"

#include <unordered_map>
#include <vector>

class Material {
public:

    Ref<Shader> m_shader = nullptr;
    
    std::unordered_map<std::string, Ref<IUniform>> uniforms = {};

    std::vector<Ref<Texture>> textures = {};

    Material(const std::string& shaderBasePath, std::vector<Ref<IUniform>> uniforms = {});
    Material(const std::string& vert, const std::string& frag, std::vector<Ref<IUniform>> uniforms = {});
    ~Material() = default;

    template <typename T = float>
    void setUniform(const std::string& name, T value) {

        // @TODO cache find() indices
        if (uniforms.find(name) != uniforms.end()) {
            auto uni = std::dynamic_pointer_cast<Uniform<T>>(uniforms.at(name));
            uni->set(value);
        } else {
            assignUniform(ref<Uniform<T>>(name, value));
        }
    }

    template <typename T = float>
    Ref<Uniform<T>> getUniform(const std::string& name) {
        return uniforms.at(name);
    }

    void assignUniform(Ref<IUniform> uniform);
    
    void assignTexture(Ref<Texture> texture);
    void assignTexture(const char* source);
    
    void bind() const;

private:
};
