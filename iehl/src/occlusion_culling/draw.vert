#version 450 core

uniform mat4 model_to_clip;

// Same location as for forward rendering.
layout(location = 1) in vec3 position;

void main() {
    gl_Position = model_to_clip * vec4(position, 1.);
}

