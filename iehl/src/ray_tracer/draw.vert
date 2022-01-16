#version 450 core

uniform mat4 object_to_clip_position;
uniform mat3 object_to_world_normal;
uniform mat4 object_to_world_position;

in vec3 a_color;
in vec3 a_normal;
in vec3 a_position;

out vec3 v_color;
out vec3 v_normal;
out vec3 v_position;

void main() {
    v_color = a_color;
    v_normal = (object_to_world_position * vec4(a_normal, 0.)).xyz;
    v_position = (object_to_world_position * vec4(a_position, 1.)).xyz;

    gl_Position = object_to_clip_position * vec4(a_position, 1.);
}
