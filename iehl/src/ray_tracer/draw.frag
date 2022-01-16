#version 450 core

uniform vec3 view_world_position;

in vec3 v_color;
in vec3 v_normal;
in vec3 v_position;

out vec3 f_color;

void main() {
    vec3 view_dir = normalize(view_world_position - v_position);

    float lambertian = abs(dot(view_dir, v_normal));

    f_color = lambertian * v_color;
}
