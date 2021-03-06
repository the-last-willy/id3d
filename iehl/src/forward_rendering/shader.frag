#version 450 core

// Unqualified normals and positions are assumed to be in world space.
// Lighting is done in world space.

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Fragment.

struct Fragment {
    vec3 position;

    // Directions.
    // In world space, outward the fragment.

    vec3 normal_dir;
    vec3 view_dir;

    // Material.

    vec3 albedo;
    vec3 emissivity;
    float opacity;
    float roughness;
    float shininess;
};

////////////////////////////////////////////////////////////////////////////////
// Inputs.

in flat uint v_draw_id;
in flat uint v_instance_index;
in vec3 v_world_normal;
in vec3 v_world_position;
in vec2 v_texcoords;

////////////////////////////////////////////////////////////////////////////////
// Outputs.

out vec4 f_rgba_color;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Modules.

////////////////////////////////////////////////////////////////////////////////
// Lighting.

struct LightProperties {
    // Cubic attenuation factors: [0] + [1] * X + [2] * X2.
    // [3] is for padding.
    vec4 attenuation;
    // In world space.
    // [3] is for padding and is expected to be '1.'.
    vec4 position;
    // [3] is for padding.
    vec4 rgb_color;

    // Direction ?
};

vec3 lighting(in Fragment f, in LightProperties light) {
    vec3 light_dir = normalize(light.position.xyz - f.position);
    float lambertian = max(dot(light_dir, f.normal_dir), 0.);

    float d = distance(light.position.xyz, f.position);
    vec3 monomial_basis = vec3(1., d, d * d);
    float inv_attenuation = dot(monomial_basis, light.attenuation.xyz);

    vec3 halfway_dir = normalize(light_dir + f.view_dir);
    float specular = pow(max(dot(halfway_dir, f.normal_dir), 0.), f.shininess);

    return (specular) * light.rgb_color.rgb / inv_attenuation;
}

////////////////////////////////////////////////////////////////////////////////
// Lighting/light_group.

layout(std430) readonly buffer light_group_light_properties_buffer {
    LightProperties light_properties[/*light index*/];
};

////////////////////////////////////////////////////////////////////////////////
// Lighting/light_group/light_culling.

struct LightCullingSpan {
    uint first;
    uint count;
};

// In world space.
uniform vec3 light_culling_domain_lower_bounds;
// In world space.
uniform vec3 light_culling_domain_upper_bounds;
uniform ivec3 light_culling_resolution;

layout(std430) readonly buffer light_culling_index_buffer {
    uint light_culling_indices[/*light culling span index*/];
};

layout(std430) readonly buffer light_culling_span_buffer {
    LightCullingSpan light_culling_spans[/*cell index*/];
};

ivec3 light_culling_cell_coords(in vec3 position) {
    vec3 lb = light_culling_domain_lower_bounds;
    vec3 ub = light_culling_domain_upper_bounds;
    ivec3 r = light_culling_resolution;

    return ivec3((position - lb) / (ub - lb) * r);
}

vec3 light_culling_lighting(in Fragment f) {
    ivec3 r = light_culling_resolution;

    ivec3 cell = light_culling_cell_coords(f.position);
    if(any(lessThan(cell, ivec3(0))) || any(greaterThanEqual(cell, r))) {
        return vec3(0.);
    } else {
        // Linearize index triplet.
        int cell_i = cell[0] * r[1] * r[2] + cell[1] * r[2] + cell[2];
        LightCullingSpan span = light_culling_spans[cell_i];

        vec3 sum = vec3(0.f);
        for(uint i = span.first; i < span.first + min(span.count, 500); ++i) {
            sum += lighting(f, light_properties[light_culling_indices[i]]);
        }
        return sum;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Material.

struct MaterialProperties {
    vec4 color_factor;
    vec4 emission_factor;
    vec4 ao_roughness_metalness_factor;
};

////////////////////////////////////////////////////////////////////////////////
// Material/material_group.

uniform sampler2DArray albedo_texture_array;
uniform sampler2DArray ao_roughness_metallic_roughness_texture_array;

layout(std430) readonly buffer material_properties_buffer {
    MaterialProperties material_properties[/*material index*/];
};

layout(std430) readonly buffer material_triangle_material_id_buffer {
    int triangle_material_ids[/*primitive index*/];
};

vec3 material_emission(in Fragment f, in uint triangle_id) {
    uint material_id = triangle_material_ids[triangle_id];
    MaterialProperties properties = material_properties[material_id];

    return properties.emission_factor.rgb;
}

vec3 material(in Fragment f, in uint triangle_id) {
    uint material_id = triangle_material_ids[triangle_id];
    MaterialProperties properties = material_properties[material_id];

    vec4 albedo_alpha = texture(
        albedo_texture_array,
        vec3(v_texcoords, material_id));

    vec3 ao_roughness_metallic = texture(
        ao_roughness_metallic_roughness_texture_array,
        vec3(v_texcoords, material_id)).rgb;
    
    return albedo_alpha.rgb;
}

////////////////////////////////////////////////////////////////////////////////
// Object.

// Only need firstIndex to calculate the triandle id.
// Don't know how to do without passing everything
// or yet another buffer containing only the indices ?
struct DrawElementsIndirectCommand {
    uint  count;
    uint  instanceCount;
    uint  firstIndex;
    uint  baseVertex;
    uint  baseInstance;
};

////////////////////////////////////////////////////////////////////////////////
// Object/object_group.

layout(std430) readonly buffer object_draw_indirect_buffer {
    DrawElementsIndirectCommand object_draw_commands[/*draw id*/];
};

uint object_global_primitive_id(in uint draw_id) {
    return object_draw_commands[draw_id].firstIndex / 3 + gl_PrimitiveID;
}

////////////////////////////////////////////////////////////////////////////////
// View.

uniform vec3 eye_world_position;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Fragment current_fragment() {
    Fragment f;

    f.position = v_world_position;

    // Directions.

    f.normal_dir = normalize(v_world_normal);
    f.view_dir = normalize(eye_world_position - v_world_position);

    // Material.

    uint triangle_id = object_global_primitive_id(v_draw_id);
    uint material_id = triangle_material_ids[triangle_id];
    MaterialProperties properties = material_properties[material_id];

    f.emissivity = properties.emission_factor.rgb;

    vec4 albedo_opacity = texture(
        albedo_texture_array,
        vec3(v_texcoords, material_id));

    f.albedo = albedo_opacity.rgb;
    f.opacity = albedo_opacity.a;

    vec3 ao_roughness_metallic = texture(
        ao_roughness_metallic_roughness_texture_array,
        vec3(v_texcoords, material_id)).rgb;
    
    f.roughness = properties.ao_roughness_metalness_factor.g
    * ao_roughness_metallic.g;

    f.shininess = 2. / pow(max(f.roughness, 0.001), 2.) - 2.;

    return f;
}

void main() {
    Fragment f = current_fragment();

    if(f.opacity <= .5) {
        discard;
    }

    float lambertian = abs(dot(f.view_dir, f.normal_dir)) * .5 + .5;

    float d = distance(f.position, eye_world_position);
    vec3 lighting = light_culling_lighting(f) / (1. + d + d);

    vec3 diffuse_lighting = lambertian * (lighting * f.albedo + f.emissivity);

    f_rgba_color = vec4(diffuse_lighting, 1.);
}
