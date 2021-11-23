#version 450 core

uniform mat4 world_to_clip;
uniform mat4 world_to_normal;

layout(location = 0) in vec3 normal;
layout(location = 1) in vec3 position;
layout(location = 2) in vec2 texcoords;

out vec3 vertex_normal;
out vec2 vertex_texcoords;

void main() {
    gl_Position = world_to_clip * vec4(position, 1.);

    vertex_normal = (world_to_normal * vec4(normal, 1.)).xyz;
    vertex_texcoords = texcoords;
}
