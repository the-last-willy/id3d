#version 450 core

uniform mat4 mvp_transform;

in vec3 v;
in vec2 vt;

out vec2 vertex_vt;

void main() {
    vertex_vt = vt;

    gl_Position = mvp_transform * vec4(v, 1.);
}
