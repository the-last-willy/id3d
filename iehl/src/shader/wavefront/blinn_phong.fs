#version 450 core

// glTF2 uniforms.
uniform vec4 baseColorFactor = vec4(1.);
uniform sampler2D baseColorTexture;

uniform vec3 light_position;
uniform vec3 view_position;

in vec3 vertex_normal;
in vec3 vertex_position;
in vec2 vertex_texcoords;

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

    vec3 albedo = baseColorFactor.xyz * texture(baseColorTexture, vertex_texcoords).xyz;

    float lambertian = max(dot(l, n), 0.);
    float specular = pow(blinn_phong_specular(h, n), 90.);

    fragment_color = (lambertian * .5) * albedo + vec3(specular);
}
