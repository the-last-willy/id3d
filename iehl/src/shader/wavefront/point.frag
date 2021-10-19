#version 450 core

in vec3 vertex_eye_position;

out vec3 fragment_color;

void main() {
    fragment_color = smoothstep(
        vec3(1., 0., 0.), vec3(0., 1., 0.), vec3(length(vertex_eye_position) / 10.));
}
