#version 450 core

uniform mat4 mvp_transform;

in vec3 POSITION;

void main() {
    gl_Position = mvp_transform * vec4(POSITION, 1.);
}
