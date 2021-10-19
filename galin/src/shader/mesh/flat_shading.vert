#version 450 core

uniform mat4 world_to_clip;
uniform mat4 world_to_eye;

in vec3 v;

out vec3 vertex_normal;
out vec3 vertex_eye_position;

void main() {
    vertex_eye_position = (world_to_eye * vec4(v, 1.)).xyz;

    gl_Position = world_to_clip * vec4(v, 1.);
}
