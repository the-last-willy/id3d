#version 450 core

uniform mat4 model_to_clip;

in vec3 position;

void main() {
    gl_Position = model_to_clip * vec4(position, 1.);
}
