#version 450 core

uniform mat4 model_to_clip;
uniform mat4 model_to_eye;

in vec3 color;
in vec3 v;

out vec3 vertex_color;
out vec3 vertex_position;

void main() {
    vertex_color = color;
    vertex_position = (model_to_eye * vec4(v, 1.)).xyz;

    gl_Position = model_to_clip * vec4(v, 1.);
}
