#version 450

in vec3 vertex_color;

out vec3 fragment_rgb;

vec3 frag_color() {
    return vertex_color;
}

void main() {
    fragment_rgb = frag_color();
}
