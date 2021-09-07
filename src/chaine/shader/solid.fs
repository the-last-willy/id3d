#version 450

in vec3 vertex_position;

out vec3 fragment_rgb;

void main() {
    vec3 normal = normalize(cross(
        dFdx(vertex_position),
        dFdy(vertex_position)));

    float lambertian = max(normal.z, 0.);

    fragment_rgb = vec3(lambertian);
}
