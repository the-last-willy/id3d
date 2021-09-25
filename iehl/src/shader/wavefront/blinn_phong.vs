#version 450 core

uniform mat4 mvp_transform;
uniform mat4 normal_transform;

in vec3 vn;
in vec3 v;
in vec2 vt;

out vec3 vertex_normal;
out vec3 vertex_position;
out vec2 vertex_texcoords;;

void main() {
    vec4 position = mvp_transform * vec4(v, 1.);

    vertex_normal = (normal_transform * vec4(vn, 0.)).xyz;
    vertex_position = position.xyz;
    vertex_texcoords = vt;

    gl_Position = position;
}
