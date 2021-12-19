#version 450 core

layout(binding = 0) readonly buffer color_buffer {
    vec4 colors[/*instances*/];
};

layout(binding = 1) readonly buffer model_to_world_buffer {
    mat4 models_to_world[/*instance*/];
};

layout(location = 0) uniform mat4 world_to_clip;
layout(location = 4) uniform mat4 world_to_view;

layout(location = 0) in vec3 normal;
layout(location = 1) in vec3 position;

out vec4 v_color;
out vec3 v_normal;
out vec3 v_position;

void main() {
    mat4 model_to_clip = world_to_clip * models_to_world[gl_InstanceID];

    v_color = colors[gl_InstanceID];
    v_normal = (world_to_view * vec4(normal, 0.)).xyz;
    v_position = (world_to_view * vec4(position, 1.)).xyz;

    gl_Position = model_to_clip * vec4(position, 1.);
}
