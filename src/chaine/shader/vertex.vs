#version 450

uniform mat4 mvp;
uniform float point_size = 1.f;

layout(location = 0) in vec3 POSITION;

void main() {
    gl_PointSize = point_size;
    gl_Position = mvp * vec4(POSITION, 1.);
}
