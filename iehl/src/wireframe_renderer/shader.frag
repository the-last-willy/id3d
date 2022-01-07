#version 450 core

uniform vec4 rgba_color = vec4(1.);

out vec4 f_rgba_color;

void main() {
    f_rgba_color = rgba_color;
}
