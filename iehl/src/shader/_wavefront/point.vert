#version 450 core

uniform mat4 mvp_transform;
uniform mat4 world_to_eye;

in vec3 color;
in vec3 v;
in vec3 vn;
in vec2 vt;

out vec3 vertex_color;
out vec3 vertex_eye_position;
out vec3 vertex_normal;
out vec2 vertex_vt;

void main() {
    vertex_color = color;
    vertex_eye_position = (world_to_eye * vec4(v, 1.f)).xyz;
    vertex_normal = normalize((mvp_transform * vec4(vn, 0.)).xyz);
    vertex_vt = vt;

    gl_Position = mvp_transform * vec4(v, 1.);
}
