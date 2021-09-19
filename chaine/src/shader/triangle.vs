#version 450

uniform mat4 mvp;

in vec3 COLOR;
in vec3 NORMAL;
in vec3 POSITION;

out vec3 vertex_color;
out vec3 vertex_normal;
out vec3 vertex_position;

void main() {
    vec4 position = mvp * vec4(POSITION, 1.);

    vertex_color = COLOR;
    vertex_normal = NORMAL;
    vertex_position = position.xyz;

    gl_Position = position;
}
