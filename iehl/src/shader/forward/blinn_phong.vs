#version 450 core

uniform mat4 mvp_transform;
uniform mat4 normal_transform;

in vec3 NORMAL;
in vec3 POSITION;

out vec3 vertex_normal;
out vec3 vertex_position;

void main() {
    vec4 position = mvp_transform * vec4(POSITION, 1.);

    vertex_normal = (normal_transform * vec4(NORMAL, 0.)).xyz;
    vertex_position = position.xyz;

    gl_Position = position;
}
