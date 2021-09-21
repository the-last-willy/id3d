#version 450 core

uniform vec4 Kd;
uniform vec3 Ks;
uniform sampler2D map_Kd;
uniform sampler2D map_Ks;

in vec2 vertex_vt;

out vec3 fragment_color;

void main() {
    // fragment_color = baseColorFactor.rgb;
    fragment_color = .5 * texture(map_Kd, vertex_vt).xyz;
}
