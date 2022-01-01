#version 450 core

#extension GL_ARB_shader_draw_parameters : require

uniform mat4 world_to_clip;

layout(location = 2) in vec2 texcoords;
layout(location = 0) in vec3 world_normal;
layout(location = 1) in vec3 world_position;

out flat uint v_draw_id;
out flat uint v_instance_no;
out vec2 v_texcoords;
out vec3 v_world_normal;
out vec3 v_world_position;

void main() {
    gl_Position = world_to_clip * vec4(position, 1.);
    v_draw_id = gl_DrawIDARB;
    v_instance_index = gl_BaseInstanceARB + gl_InstanceID;
    v_texcoords = texcoords;
    v_world_normal = world_normal;
    v_world_position = world_position;
}
