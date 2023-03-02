#pragma once

#include "common.h"
#include "gfx/Shader.h"
#include "gfx/Uniforms.h"

#include <unordered_map>
#include <vector>

class Material {
public:

    Ref<Shader> m_shader;
    
    std::unordered_map<std::string, IUniform*> uniforms = {};

    Material(const std::string& shaderBasePath, std::vector<IUniform*> uniforms = {});
    ~Material() = default;

    template <typename T = float>
    void setUniform(const std::string& name, T value) {
        if (uniforms.find(name) == uniforms.end()) {
            this->assignUniform(new Uniform<T>(name, value));
        } else {
            static_cast<Uniform<T>*>(this->uniforms.at(name))->set(value);
        }
    }

    template <typename T = float>
    Uniform<T>* getUniform(const std::string& name) {
        return static_cast<Uniform<T>*>(this->uniforms.at(name));
    }

    void assignUniform(IUniform* uniform);
    
    void bind() const;

private:
};
