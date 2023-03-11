#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D framebuffer;

void main() {

	vec4 color = texture(framebuffer, uv);

	// color.g = 0.0;

	FragColor = color;

}
