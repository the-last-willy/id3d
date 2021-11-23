#version 450 core

in vec3 vertex_normal;
in vec2 vertex_texcoords;

out vec4 fragment_color;

void main() {
    vec3 normal = normalize(vertex_normal);

    vec3 color = normal * 0.5 + 0.5;
    fragment_color = vec4(color, 1.);
}
