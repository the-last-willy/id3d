#version 450

in vec3 vertex_eye_position;

out vec3 fragment_rgb;

// Normal.

vec3 flat_normal() {
    return normalize(cross(
        dFdx(vertex_eye_position),
        dFdy(vertex_eye_position)));
}

// vec3 smooth_normal() {
//     return normalize(vertex_normal);
// }

//

void main() {
    vec3 normal = flat_normal();

    float lambertian = abs(normal.z);

    fragment_rgb = vec3(lambertian) * .5 + .5;
}
