#version 450 core

uniform vec4 rgba_color;
uniform vec3 view_world_position;

in vec3 v_world_normal;
in vec3 v_world_position;

out vec4 f_rgba_color;

void main() {
    vec3 normal_dir = normalize(v_world_normal);
    vec3 view_dir = normalize(view_world_position - v_world_position);

    // float lambertian = .5; 
    float lambertian = abs(dot(normal_dir, view_dir)) * .5 + .5;
    f_rgba_color = lambertian * rgba_color;
}