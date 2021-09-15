#version 450 core

uniform sampler2D baseColorTexture;

in vec2 vertex_texcoords;

out vec3 fragment_color;

void main() {
    fragment_color = .5 * texture(baseColorTexture, vertex_texcoords).xyz;
}
