#version 450 core

#extension GL_ARB_shader_draw_parameters : require

uniform mat4 world_to_clip_position;

layout(location = 2) in vec2 a_texcoords;
layout(location = 1) in vec3 a_world_position;

out flat uint v_draw_id;
out vec2 v_texcoords;

void main() {
    v_draw_id = gl_DrawIDARB;
    v_texcoords = a_texcoords;

    gl_Position = world_to_clip_position * vec4(a_world_position, 1.);
}
