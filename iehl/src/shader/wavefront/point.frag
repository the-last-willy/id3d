#version 450 core

in vec3 vertex_color;
in vec3 vertex_eye_position;
in vec3 vertex_normal;

out vec3 fragment_color;

void main() {
    vec3 normal = normalize(vertex_normal);

    float lambertian = max(normal.z, 0.);

    fragment_color = vertex_color;
    // fragment_color = smoothstep(
    //     vec3(1., 0., 0.), vec3(0., 1., 0.), vec3(length(vertex_eye_position) / 10.));
}
