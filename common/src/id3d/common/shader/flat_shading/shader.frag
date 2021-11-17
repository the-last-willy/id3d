#version 450

uniform vec4 color_factor = vec4(1.);

in vec3 vertex_color;
in vec3 vertex_position;

out vec4 fragment_color;

// Normal.

vec3 flat_normal() {
    return normalize(cross(
        dFdx(vertex_position),
        dFdy(vertex_position)));
}

//

void main() {
    vec3 normal = flat_normal();

    float lambertian = abs(normal.z);

    fragment_color = color_factor * vec4(vec3(lambertian) * .5 + .5, 1.) * vec4(vertex_color, 1.);
}
