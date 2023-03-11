
#include "common.h"
#include "gfx/Uniforms.h"

template<> void Uniform<int>       ::bind() { if(m_needsUpdate) { m_needsUpdate = false; glUniform1i(m_location, m_value); }}
template<> void Uniform<float>     ::bind() { if(m_needsUpdate) { m_needsUpdate = false; glUniform1f(m_location, m_value); }}
template<> void Uniform<glm::vec3> ::bind() { if(m_needsUpdate) { m_needsUpdate = false; glUniform3f(m_location, m_value.x, m_value.y, m_value.z); }}
template<> void Uniform<glm::vec4> ::bind() { if(m_needsUpdate) { m_needsUpdate = false; glUniform4f(m_location, m_value.x, m_value.y, m_value.z, m_value.w); }}
template<> void Uniform<glm::mat4> ::bind() { if(m_needsUpdate) { m_needsUpdate = false; glUniformMatrix4fv(m_location, 1, GL_FALSE, &m_value[0][0]); }}
