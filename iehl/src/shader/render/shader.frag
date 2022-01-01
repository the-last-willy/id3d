#version 450 core

struct Material {
    vec4 color_factor;
    vec4 emission_factor;
};

struct Span {
    uint offset;
    uint count;
};

layout(binding = 0) uniform sampler2DArray albedo_array_texture;

layout(std430, binding = 0) buffer material_ids {
    int triangle_material_ids[/*primitive index*/];
};

layout(std430, binding = 1) buffer material_buffer {
    Material materials[/*material index*/];
};

layout(std430, binding = 2) buffer primitive_offset_buffer {
    uint primitive_offsets[/*draw id*/]; 
};

// Lights.

struct Light {
    // Cubic attenuation factors: [0] * X2 + [1] * X + [2].
    // [3] is for padding.
    vec4 attenuation;
    // [3] is for padding.
    vec4 color;
    // In view space.
    // [3] is for padding and should be '1.'.
    vec4 position;

    // Direction ?
};

// Not required for light culling.
uniform uint light_count = 0;

layout(std430, binding = 3) readonly buffer light_buffer {
    Light lights[];
};

vec3 lighting(in Light l, in vec3 position) {
    float d = distance(l.position.xyz, position);
    vec3 monomial_basis = vec3(1., d, d * d);
    float attenuation = dot(monomial_basis, l.attenuation.xyz);
    return l.color.rgb / attenuation;
}

// Light culling.

struct LightCullingSpan {
    uint first;
    uint count;
};

// World space.
uniform vec3 light_culling_domain_lower_bound;
// World space.
uniform vec3 light_culling_domain_upper_bound;
uniform vec3 light_culling_resolution;

layout(std430, binding = 4) readonly buffer light_culling_index_buffer {
    uint light_culling_indices[/*light culling span point*/];
};

layout(std430, binding = 5) readonly buffer light_culling_span_buffer {
    LightCullingSpan light_culling_spans[/*cell index*/];
};

vec3 light_culling_lighting(in vec3 position) {
    // Aliases.

    vec3 lb = light_culling_domain_lower_bound;
    vec3 ub = light_culling_domain_upper_bound;
    vec3 r = light_culling_resolution;

    // Retrieve the lights.

    vec3 indices = trunc((position - lb) / (ub - lb) * r);
    uint cell_i = uint(indices[0] * r[1] * r[2] + indices[1] * r[2] * indices[2]);
    LightCullingSpan span = light_culling_spans[cell_i];

    // Accumulate the lighting.

    vec3 sum = vec3(0.);
    for(uint i = span.first; i < span.first + span.count; ++i) {
        sum += lighting(lights[i], position);
    }
    return sum;
}

//

in flat uint v_draw_id;
in flat uint v_instance_index;
// World space.
in vec3 v_normal;
// World space.
in vec3 v_position;
in vec2 v_texcoords;

out vec4 fragment_color;

void main() {
    uint primitive_id = primitive_offsets[v_draw_id] + gl_PrimitiveID;

    triangle_material_ids.length;

    vec3 normal = normalize(v_normal);

    vec3 lighting = vec3(0.);

    int material_id = triangle_material_ids[primitive_id];
    if(material_id != -1) {
        const Material material = materials[material_id];
        vec4 texel = texture(albedo_array_texture, vec3(v_texcoords, material_id));
        // fragment_color = material.emission_factor * material.color_factor * texel + vec4(lighting, 0.);
        fragment_color = 2. * material.emission_factor * material.color_factor * texel + vec4(lighting, 0.);
        // fragment_color = vec4(vec3(material.emission_factor), 1.) + vec4(lighting, 0.);
    } else {
        fragment_color = vec4(vec3(1., 1., 1.), 1.);
    }

    fragment_color = vec4(normal * .5 + .5, 1.);
}
