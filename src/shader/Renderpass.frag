#version 330 core

in vec2 uv;

uniform sampler2D framebuffer;

void main() {

	vec4 color = texture(framebuffer, uv);

	// color.g = 0.0;

	gl_FragColor = color;

}
