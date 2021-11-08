#version 450

in vec3 vertex_position;

out vec4 fragment_color;

// Normal.

vec3 flat_normal() {
    return normalize(cross(
        dFdx(vertex_position),
        dFdy(vertex_position)));
}

// vec3 smooth_normal() {
//     return normalize(vertex_normal);
// }

//

void main() {
    vec3 normal = flat_normal();

    float lambertian = abs(normal.z);

    fragment_color = vec4(vec3(lambertian) * .5 + .5, 1.);
}
