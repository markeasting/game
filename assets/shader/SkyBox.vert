#version 330 core
layout (location = 0) in vec3 v_position;

out vec3 uv;

// uniform mat4 u_modelViewProjectionMatrix;
// uniform mat4 u_modelViewMatrix;
uniform mat4 u_viewRotationMatrix;
uniform mat4 u_projectionMatrix;

void main()
{
    uv = -v_position;
    
    vec4 pos = u_projectionMatrix * u_viewRotationMatrix * vec4(v_position, 1.0);
    gl_Position = pos.xyww;
} 
