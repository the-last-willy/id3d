#version 450 core

uniform vec3 light_position;
uniform vec3 view_position;

in vec3 vertex_normal;
in vec3 vertex_position;

out vec3 fragment_color;

float blinn_phong_specular(vec3 h, vec3 n) {
    return max(dot(h, n), 0.);
}

float phong_specular(vec3 l, vec3 n, vec3 v) {
    return max(dot(v, -reflect(l, n)), 0.);
}

void main() {
    // Directions.
    vec3 n = normalize(vertex_normal);
    vec3 l = normalize(light_position - vertex_position);
    vec3 v = normalize(view_position - vertex_position);
    vec3 h = normalize(l + v);

    float lambertian = max(dot(l, n), 0.);
    float specular = pow(phong_specular(l, n, v), 30.);

    fragment_color = vec3(lambertian + specular);
}
