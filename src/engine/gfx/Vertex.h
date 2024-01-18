#pragma once

#include "engine/common/glm.h"

struct Vertex {
    
    vec3 position = vec3(0);
    vec3 normal   = vec3(0);
    vec2 uv       = vec2(0);
    // vec4 color    = vec4(0);

    Vertex();
    
    Vertex(vec3 position = vec3(0), vec3 normal = vec3(0), vec2 uv = vec2(0));
    Vertex(float x = 0, float y = 0, float z = 0);
	
    ~Vertex() = default;

    static void setAttribPointers();

};