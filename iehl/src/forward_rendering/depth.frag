#version 450 core

////////////////////////////////////////////////////////////////////////////////
// Material/group.

uniform sampler2DArray material_albedo_opacity;

layout(std430) readonly buffer material_triangle_material_id_buffer {
    int material_triangle_material_ids[/*primitive index*/];
};

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
// Object/group.

layout(std430) readonly buffer object_draw_indirect_buffer {
    DrawElementsIndirectCommand object_draw_commands[/*draw id*/];
};

uint object_global_primitive_id(in uint draw_id) {
    return object_draw_commands[draw_id].firstIndex / 3 + gl_PrimitiveID;
}

////////////////////////////////////////////////////////////////////////////////

in flat uint v_draw_id;
in vec2 v_texcoords;

out vec3 f_color;

void main() {
    uint primitive_id = object_global_primitive_id(v_draw_id);
    uint material_id = material_triangle_material_ids[primitive_id];
    vec4 albedo_opacity = texture(material_albedo_opacity, vec3(v_texcoords, material_id));
    float opacity = albedo_opacity.a;
    if(opacity <= 0.5) {
        discard;
    }
}
