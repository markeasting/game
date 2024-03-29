#version 330 core
// https://github.com/paulpela/lighting-opengl/tree/master/shaders

in VS_OUT {
    vec3 N;
    vec3 L;
    vec3 V;
} fs_in;

uniform vec3 ambient = vec3(0.15, 0.0, 0.0);
uniform vec3 diffuseAlbedo = vec3(0.7, 0.1, 0.0);
uniform vec3 specularAlbedo = vec3(1.0, 0.8, 0.8);
uniform vec3 rimColor = vec3(0.3, 0.3, 0.3) * 2.0;

uniform float shininess = 100.0;
uniform float rimPower = 4.0;

uniform int specularOn = 1;
uniform int rimLightOn = 1;

out vec4 out_color;

vec3 rimLight(vec3 N, vec3 V) {

    float f = 1.0 - dot(N, V);

    f = smoothstep(0.0, 1.0, f);

    f = pow(f, rimPower);

    return f * rimColor;
}

void main() {
    vec3 N = normalize(fs_in.N);
    vec3 L = normalize(fs_in.L);
    vec3 V = normalize(fs_in.V);
    vec3 R = reflect(-L, N);

    vec3 diffuse = max(dot(N, L), 0.0) * diffuseAlbedo;

    vec3 specular = vec3(0.0);

    if (specularOn == 1) {
        specular = pow(max(dot(R, V), 0.0), shininess) * specularAlbedo;
    }

    if (rimLightOn == 1) {
        out_color = vec4(ambient + diffuse + specular + rimLight(N, V), 1.0);
    } else {
        out_color = vec4(ambient + diffuse + specular, 1.0);
    }
}
