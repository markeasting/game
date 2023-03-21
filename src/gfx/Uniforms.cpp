
#include "common.h"
#include "gfx/Uniforms.h"

/* @TODO check if the branching of m_needsUpdate actually decreases performance. */
template<> void Uniform<int>   ::bind() { if(m_needsUpdate) { m_needsUpdate = false; glUniform1i(m_location, m_value); }}
template<> void Uniform<float> ::bind() { if(m_needsUpdate) { m_needsUpdate = false; glUniform1f(m_location, m_value); }}
template<> void Uniform<vec3>  ::bind() { if(m_needsUpdate) { m_needsUpdate = false; glUniform3f(m_location, m_value.x, m_value.y, m_value.z); }}
template<> void Uniform<vec4>  ::bind() { if(m_needsUpdate) { m_needsUpdate = false; glUniform4f(m_location, m_value.x, m_value.y, m_value.z, m_value.w); }}
template<> void Uniform<mat4>  ::bind() { if(m_needsUpdate) { m_needsUpdate = false; glUniformMatrix4fv(m_location, 1, GL_FALSE, value_ptr(m_value)); }}
