
#include "common.h"
#include "gfx/Uniforms.h"

template<> void Uniform<int>       ::bind() { if(_update) { _update = false; glUniform1i(this->location, this->value); }}
template<> void Uniform<float>     ::bind() { if(_update) { _update = false; glUniform1f(this->location, this->value); }}
template<> void Uniform<glm::vec3> ::bind() { if(_update) { _update = false; glUniform3f(this->location, this->value.x, this->value.y, this->value.z); }}
template<> void Uniform<glm::vec4> ::bind() { if(_update) { _update = false; glUniform4f(this->location, this->value.x, this->value.y, this->value.z, this->value.w); }}
template<> void Uniform<glm::mat4> ::bind() { if(_update) { _update = false; glUniformMatrix4fv(this->location, 1, GL_FALSE, &this->value[0][0]); }}
