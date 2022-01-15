#version 450 core

uniform mat4 world_to_clip;

in vec3 a_world_position;

void main() {
    gl_Position = world_to_clip * vec4(a_world_position, 1.);
}
