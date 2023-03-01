#pragma once

#include "common.h"
#include "gfx/Uniforms.h"

#include <string>
#include <unordered_map>

class Shader {
public:
    std::string m_vertexShaderPath;
    std::string m_fragmentShaderPath;
    GLuint m_program = 0;
    int u_mvp_location = -1;

    Shader(); 
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, bool autoCompile = true);
    Shader(const std::string& vertexShaderBasePath, bool autoCompile = true);
    ~Shader();
    
    GLuint compile(const std::string& shaderSource, unsigned int type);
    GLuint createProgram();

    int getUniformLocation(const std::string& name);
    // int getProjectionMatrixLocation();
    
    const void bind() const;
    const void unBind() const;

private:
    std::unordered_map<std::string, int> uniformLocationCache = {};
};
