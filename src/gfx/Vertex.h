#pragma once

#include "common_includes.h"

struct Vertex {
    
    glm::vec3 position = glm::vec3(0);
    glm::vec3 normal   = glm::vec3(0);
    glm::vec2 uv       = glm::vec2(0);
    // glm::vec4 color    = glm::vec4(0);

    Vertex();
    // Vertex(glm::vec3 position);
    // Vertex(glm::vec3 position, glm::vec3 normal);
    Vertex(glm::vec3 position = glm::vec3(0), glm::vec3 normal = glm::vec3(0), glm::vec2 uv = glm::vec2(0));
    Vertex(float x = 0, float y = 0, float z = 0);
	~Vertex() = default;

    static void setAttribPointers();

};