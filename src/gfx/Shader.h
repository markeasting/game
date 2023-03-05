#pragma once

#include "common_includes.h"
#include "gfx/Uniforms.h"

#include <string>
#include <unordered_map>

class Shader {
public:
    std::string m_vertexShaderPath;
    std::string m_fragmentShaderPath;
    GLuint m_program = 0;

    Shader(); 
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, bool autoCompile = true);
    Shader(const std::string& vertexShaderBasePath, bool autoCompile = true);
    ~Shader();

    int getUniformLocation(const std::string& name);
    
    const void bind() const;
    const void unBind() const;

private:
    std::unordered_map<std::string, int> uniformLocationCache = {};

    GLuint compile(const std::string& shaderSource, unsigned int type);
    GLuint createProgram();
};
