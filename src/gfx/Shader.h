#pragma once

#include "common.h"
#include "gfx/Uniforms.h"

class Shader {
public:
    GLuint m_program = 0;

    Shader(); 
    Shader(const std::string& vertexShader, const std::string& fragmentShader, bool autoCompile = true);
    Shader(const std::string& shaderName, bool autoCompile = true);
    ~Shader();

    int getUniformLocation(const std::string& name);
    
    const void bind() const;
    const void unBind() const;

private:

    static constexpr const char* shaderBasePath = "/assets/shader/";

    std::string m_vertexShaderPath;
    std::string m_fragmentShaderPath;

    std::unordered_map<std::string, int> uniformLocationCache = {};

    GLuint createProgram();
    GLuint compile(const std::string& shaderSource, unsigned int type);
};
