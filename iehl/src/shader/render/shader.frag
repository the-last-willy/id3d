#version 450 core

struct Material {
    vec4 color_factor;
};

layout(binding = 0) uniform sampler2DArray albedo_array_texture;

layout(std430, binding = 0) buffer material_ids {
    int triangle_material_ids[];
};

layout(std430, binding = 1) buffer material_buffer {
    Material materials[];
};

in vec3 vertex_normal;
in vec2 vertex_texcoords;

out vec4 fragment_color;

void main() {
    triangle_material_ids.length;

    vec3 normal = normalize(vertex_normal);

    vec3 color = normal * 0.5 + 0.5;
    fragment_color = vec4(color, 1.);

    int material_id = triangle_material_ids[gl_PrimitiveID];
    if(material_id != -1) {
        const Material material = materials[material_id];
        vec4 texel = texture(albedo_array_texture, vec3(vertex_texcoords, material_id));
        fragment_color = material.color_factor * texel;
    } else {
        fragment_color = vec4(vec3(1., 1., 1.), 1.);
    }
}
