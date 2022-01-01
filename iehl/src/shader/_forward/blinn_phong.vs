#version 450 core

uniform mat4 mvp_transform;
uniform mat4 normal_transform;

in vec3 NORMAL;
in vec3 POSITION;
in vec2 TEXCOORD_0;

out vec3 vertex_normal;
out vec3 vertex_position;
out vec2 vertex_texcoords;;

void main() {
    vec4 position = mvp_transform * vec4(POSITION, 1.);

    vertex_normal = (normal_transform * vec4(NORMAL, 0.)).xyz;
    vertex_position = position.xyz;
    vertex_texcoords = TEXCOORD_0;

    gl_Position = position;
}
