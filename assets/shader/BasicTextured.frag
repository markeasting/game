#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float u_opacity = 1.0;

void main() {

	vec4 textureColor = texture(texture1, uv);
	FragColor = vec4(textureColor.rgb, textureColor.a * u_opacity);

	// FragColor = mix(
	// 	texture(texture1, uv), 
	// 	texture(texture2, uv), 
	// 	0.5
	// );

}
