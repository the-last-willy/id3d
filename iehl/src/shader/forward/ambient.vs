#version 450 core

uniform mat4 mvp_transform;

in vec3 POSITION;
in vec2 TEXCOORD_0;

out vec2 vertex_texcoords;

void main() {
    vertex_texcoords = TEXCOORD_0;

    gl_Position = mvp_transform * vec4(POSITION, 1.);
}
