#version 450 core

uniform mat4 world_to_clip;

in vec3 position;

void main() {
    gl_Position = world_to_clip * vec4(position, 1.);
}
