#version 450 core

// Unqualified normals and positions are assumed to be in world space.
// Lighting is done in world space.

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Interface.

////////////////////////////////////////////////////////////////////////////////
// Geometry.

in flat uint v_draw_id;
in flat uint v_instance_index;
in vec3 v_world_normal;
in vec3 v_world_position;
in vec2 v_texcoords;

////////////////////////////////////////////////////////////////////////////////
// Light.

struct Light {
    // Cubic attenuation factors: [0] + [1] * X + [2] * X2.
    // [3] is for padding.
    vec4 attenuation;
    // [3] is for padding.
    vec4 rgb_color;
    // In world space.
    // [3] is for padding and is expected to be '1.'.
    vec4 position;

    // Direction ?
};

vec3 lighting(in Light l, in vec3 position) {
    float d = distance(l.position.xyz, position);
    vec3 monomial_basis = vec3(1., d, d * d);
    float attenuation = dot(monomial_basis, l.attenuation.xyz);
    return l.rgb_color.rgb / attenuation;
}

////////////////////////////////////////////////////////////////////////////////
// Light/group.

layout(std430, binding = 3) readonly buffer light_buffer {
    Light lights[];
};

////////////////////////////////////////////////////////////////////////////////
// Light/group/culling.

struct LightCullingSpan {
    uint first;
    uint count;
};

// In world space.
uniform vec3 light_culling_domain_lower_bounds;
// In world space.
uniform vec3 light_culling_domain_upper_bounds;
uniform ivec3 light_culling_resolution;

layout(std430, binding = 4) readonly buffer light_culling_index_buffer {
    uint light_culling_indices[/*light culling span point*/];
};

layout(std430, binding = 5) readonly buffer light_culling_span_buffer {
    LightCullingSpan light_culling_spans[/*cell index*/];
};

// Returns '-1' if position does not belong to the domain.
int light_culling_cell_index(in vec3 position) {
    vec3 lbs = light_culling_domain_lower_bound;
    vec3 ubs = light_culling_domain_upper_bound;
    vec3 r = light_culling_resolution;

    ivec3 cell = ivec3(trunc((position - lb) / (ub - lb) * r));
    if(any(lessThan(cell, ivec3(0))) || any(greaterThanEqual(cell, r))) {
        return -1;
    } else {
        return int(indices[0] * r[1] * r[2] + indices[1] * r[2] * indices[2]);
    }
}

vec3 light_culling_lighting(in vec3 position) {
    int cell_i = light_culling_cell_index(position);
    LightCullingSpan span = light_culling_spans[cell_i];

    vec3 sum = vec3(0.);
    for(uint i = span.first; i < span.first + span.count; ++i) {
        sum += lighting(lights[i], position);
    }
    return sum;
}

////////////////////////////////////////////////////////////////////////////////
// Material.

struct Material {
    vec4 color_factor;
    vec4 emission_factor;
};

////////////////////////////////////////////////////////////////////////////////
// Material/group.

struct Span {
    uint offset;
    uint count;
};

layout(binding = 0) uniform sampler2DArray albedo_array_texture;

layout(std430) buffer material_ids {
    int triangle_material_ids[/*primitive index*/];
};

layout(std430) buffer material_buffer {
    Material materials[/*material index*/];
};

layout(std430) buffer primitive_offset_buffer {
    uint primitive_offsets[/*draw index*/]; 
};

////////////////////////////////////////////////////////////////////////////////
// Fragment output.

out vec4 f_rgba_color;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void main() {
    // uint primitive_id = primitive_offsets[v_draw_id] + gl_PrimitiveID;

    // triangle_material_ids.length;

    // vec3 normal = normalize(v_normal);

    // vec3 lighting = vec3(0.);

    // int material_id = triangle_material_ids[primitive_id];
    // if(material_id != -1) {
    //     const Material material = materials[material_id];
    //     vec4 texel = texture(albedo_array_texture, vec3(v_texcoords, material_id));
    //     // f_rgba_color = material.emission_factor * material.color_factor * texel + vec4(lighting, 0.);
    //     f_rgba_color = 2. * material.emission_factor * material.color_factor * texel + vec4(lighting, 0.);
    //     // f_rgba_color = vec4(vec3(material.emission_factor), 1.) + vec4(lighting, 0.);
    // } else {
    //     f_rgba_color = vec4(vec3(1., 1., 1.), 1.);
    // }

    // f_rgba_color = vec4(normal * .5 + .5, 1.);

    f_rgba_color = vec4(1.);
}
