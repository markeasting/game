#pragma once

#include "engine/gfx/Shader.h"
#include "engine/gfx/Uniforms.h"
#include "engine/gfx/Texture.h"

#include <vector>

class Material {
public:

    Ref<Shader> m_shader = nullptr;
    
    std::unordered_map<std::string, Ref<IUniform>> uniforms = {};

    std::vector<Ref<Texture>> textures = {};

    bool wireframe = false;
    bool transparent = false;

    Material(
        const std::string& shaderBasePath, 
        std::vector<Ref<IUniform>> uniforms = {}
    );
    
    Material(
        const std::string& vert, 
        const std::string& frag, 
        std::vector<Ref<IUniform>> uniforms = {}
    );

    ~Material() = default;

    template <typename T>
    void setUniform(const std::string& name, T value) {

        // @TODO cache find() indices
        if (uniforms.find(name) != uniforms.end()) {
            auto uni = std::static_pointer_cast<Uniform<T>>(uniforms.at(name));
            uni->set(value);
        } else {
            assignUniform(ref<Uniform<T>>(name, value));
        }
    }

    template <typename T>
    Ref<Uniform<T>> getUniform(const std::string& name) {

        if (uniforms.find(name) != uniforms.end()) {
            return std::static_pointer_cast<Uniform<T>>(uniforms.at(name));
        } else {
            throw std::runtime_error("uniform not found: " + name);
        }
    }

    void assignUniform(Ref<IUniform> uniform);
    
    void assignTexture(
        Ref<Texture> texture, 
        const std::string& uniform = "texture"
    );

    void assignTexture(
        const char* source, 
        const std::string& uniform = "texture"
    );
    
    void bind() const;

private:
};
