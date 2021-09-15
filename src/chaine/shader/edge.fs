#version 450

uniform vec3 color = vec3(1.);

in vec3 vertex_position;

out vec3 fragment_rgb;

void main() {
    fragment_rgb = color;
}
