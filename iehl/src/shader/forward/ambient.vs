#version 450 core

uniform mat4 mvp_transform;

in vec3 POSITION;

void main() {
    vec4 position = mvp_transform * vec4(POSITION, 1.);

    gl_Position = position;
}
