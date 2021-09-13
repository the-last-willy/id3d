#version 450

in vec3 vertex_color;
in vec3 vertex_normal;
in vec3 vertex_position;

out vec3 fragment_rgb;

// Normal.

vec3 flat_normal() {
    return normalize(cross(
        dFdx(vertex_position),
        dFdy(vertex_position)));
}

vec3 smooth_normal() {
    return normalize(vertex_normal);
}

//

vec3 frag_color() {
    return vertex_color;
}

void main() {
    vec3 normal = flat_normal();

    float lambertian = abs(normal.z);

    fragment_rgb = frag_color();
}
