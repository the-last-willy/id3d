#version 450

uniform mat4 mvp;
uniform float point_size = 1.f;

in vec3 COLOR;
in vec3 POSITION;

out vec3 vertex_color;

void main() {
    vertex_color = COLOR;

    gl_PointSize = point_size;
    gl_Position = mvp * vec4(POSITION, 1.);
}
