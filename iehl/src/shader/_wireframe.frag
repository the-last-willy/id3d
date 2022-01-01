#version 450 core

uniform vec3 rgb = vec3(1.);

out vec3 fragment_rgb;

void main() {
    fragment_rgb = rgb;
}
