#pragma once

#include "common.h"
#include "gfx/Shader.h"
#include "gfx/Uniforms.h"

#include <unordered_map>
#include <vector>

class Material {
public:
    Shader* shader = nullptr;
    
    std::unordered_map<std::string, IUniform*> uniforms = {};

    Material();
    Material(const std::string& shaderBasePath, std::vector<IUniform*> uniforms = {});
    ~Material() = default;

    void setShader(Shader* shader);

    template <typename T = int>
    void setUniform(const std::string& name, T value) {
        if (uniforms.find(name) == uniforms.end()) {
            this->assignUniform(new Uniform<T>(name, value));
        } else {
            static_cast<Uniform<T>*>(this->uniforms.at(name))->set(value);
        }
    }

    template <typename T = int>
    Uniform<T>* getUniform(const std::string& name) {
        return static_cast<Uniform<T>*>(this->uniforms.at(name));
    }

    void bind();

    void assignUniform(IUniform* uniform);
private:
};
