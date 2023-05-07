#version 330 core
out vec4 FragColor;

in vec3 uv;

uniform samplerCube texture1;

void main() {
    FragColor = texture(texture1, uv);
}
