#version 450 core

#extension GL_ARB_shader_draw_parameters : require

uniform mat4 model_to_clip;
uniform mat4 model_to_world;
uniform mat4 model_to_world_normal;

layout(location = 0) in vec3 normal;
layout(location = 1) in vec3 position;
layout(location = 2) in vec2 texcoords;

out flat uint v_draw_id;
out flat uint v_instance_no;
out vec2 v_texcoords;
out vec3 v_world_normal;
out vec3 v_world_position;

void main() {
    gl_Position = model_to_clip * vec4(position, 1.);
    
    v_draw_id = gl_DrawIDARB;
    v_instance_index = gl_BaseInstanceARB + gl_InstanceID;
    v_texcoords = texcoords;
    v_world_normal = (model_to_world_normal * vec4(normal, 0.)).xyz;
    v_world_position = (model_to_world * vec4(position, 1.)).xyz;
}
