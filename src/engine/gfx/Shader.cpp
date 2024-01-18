
#include "engine/gfx/Shader.h"
#include "engine/util/Filesystem.h"

Shader::Shader() {}

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader, bool autoCompile) 
    : m_vertexShaderPath(vertexShader), m_fragmentShaderPath(fragmentShader)
{
    if(autoCompile)
        m_program = createProgram();
}

Shader::Shader(const std::string& shaderName, bool autoCompile) {
    m_vertexShaderPath = shaderName + ".vert";
    m_fragmentShaderPath = shaderName + ".frag";

    if(autoCompile)
        m_program = createProgram();
}

Shader::~Shader() {
    glDeleteProgram(m_program);
}

GLuint Shader::createProgram() {

    GLuint vert = compile(Shader::shaderBasePath + m_vertexShaderPath, GL_VERTEX_SHADER);
    GLuint frag = compile(Shader::shaderBasePath + m_fragmentShaderPath, GL_FRAGMENT_SHADER);

    if(vert == 0 || frag == 0) {
        return 0;
    }

    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vert);
    glAttachShader(program_id, frag);
    glLinkProgram(program_id);
    glValidateProgram(program_id);
    glDeleteShader(vert);
    glDeleteShader(frag);

    int program_linked = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &program_linked);

    if (program_linked == GL_FALSE) {

        GLsizei log_length = 0;
        GLchar message[1024];
        
        glGetProgramInfoLog(program_id, 1024, &log_length, message);

        printf("[ERROR] Shader linking error: %s\n", message);

        glDeleteProgram(program_id);
        glDeleteShader(vert);
        glDeleteShader(frag);

        return 0;
    }

    // printf("[SUCCESS] Shader ID %u compiled.\n", program_id);

    glDetachShader(program_id, vert);
    glDetachShader(program_id, frag);

    return program_id;
}

GLuint Shader::compile(const std::string& shaderSource, unsigned int type) {

    Filesystem &fs = Filesystem::instance();
    std::string shader_str = fs.getFileContents(shaderSource);

    GLuint shaderId = glCreateShader(type);
    const char* src = shader_str.c_str();
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    int compilation_result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilation_result);

    if(compilation_result == GL_FALSE) {
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        
        char* message = (char*) malloc(length * sizeof(char));
        glGetShaderInfoLog(shaderId, length, &length, message);

        printf("[ERROR] Could not compile shader: '%s'. \nOpenGL: %s", shaderSource.c_str(), message);
        
        glDeleteShader(shaderId);
        
        return 0;
    }

    return shaderId;
}


int Shader::getUniformLocation(const std::string& name) {

    if(uniformLocationCache.find(name) == uniformLocationCache.end()) {
        glUseProgram(m_program);
        int location = glGetUniformLocation(m_program, name.c_str());
        // std::cout << name << std::endl;
        // assert(location != -1);
        uniformLocationCache[name] = location;
    }

    return uniformLocationCache[name];
}

const void Shader::bind() const {
    glUseProgram(m_program);
}

const void Shader::unBind() const {
    glUseProgram(0);
}
