#version 450 core

uniform vec3 color = vec3(1.);

out vec3 fragment_color;

void main() {
    fragment_color = color;
}
