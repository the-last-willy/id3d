#version 450 core

uniform mat4 object_to_clip_position;
uniform mat3 object_to_world_normal;
uniform mat4 object_to_world_position;

in vec3 a_normal;
in vec3 a_position;

out vec3 v_world_normal;
out vec3 v_world_position;

void main() {
    // Should be 'object_to_world_normal' but I'm lazy.
    v_world_normal = (object_to_world_position * vec4(a_normal, 0.)).xyz;

    vec4 world_position = object_to_world_position * vec4(a_position, 1.);
    v_world_position = world_position.xyz  / world_position.w;

    gl_Position = object_to_clip_position * vec4(a_position, 1.);
}
