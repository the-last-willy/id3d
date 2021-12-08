#version 450 core

#extension GL_ARB_shader_draw_parameters : require

uniform mat4 world_to_view;
uniform mat4 world_to_clip;
uniform mat4 world_to_normal;

layout(location = 0) in vec3 normal;
layout(location = 1) in vec3 position;
layout(location = 2) in vec2 texcoords;

out flat uint vertex_draw_id;
out flat uint vertex_instance_index;
out vec3 vertex_normal;
out vec3 vertex_position;
out vec2 vertex_texcoords;

void main() {
    gl_Position = world_to_clip * vec4(position, 1.);

    vertex_draw_id = gl_DrawIDARB;
    vertex_instance_index = gl_BaseInstanceARB + gl_InstanceID;
    vertex_normal = (world_to_normal * vec4(normal, 1.)).xyz;
    vertex_position = (world_to_view * vec4(position, 1.)).xyz;
    vertex_texcoords = texcoords;
}
