#version 450 core

uniform mat4 model_to_clip;

in vec3 a_position;

void main() {
    gl_Position = model_to_clip * vec4(a_position, 1.);
}
