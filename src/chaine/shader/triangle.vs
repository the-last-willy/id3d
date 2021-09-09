#version 450

uniform mat4 mvp;

layout(location = 0) in vec3 POSITION;

out vec3 vertex_position;

void main() {
    vec4 position = mvp * vec4(POSITION, 1.);

    vertex_position = position.xyz;

    gl_Position = position;
}
