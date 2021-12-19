#version 450 core

in vec4 v_color;
in vec3 v_normal;
in vec3 v_position;

out vec4 f_color;

void main() {
    f_color = v_color;
}