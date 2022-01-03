#pragma once

#include "lighting/all.hpp"
#include "material/all.hpp"
#include "object/all.hpp"
#include "forward_renderer.hpp"

inline
void upload(const ForwardRenderer& fr, const LightCulling& lc) {
    // SSBOs.

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        fr.light_culling_index_buffer_binding,
        lc.light_index_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        fr.light_culling_span_buffer_binding,
        lc.light_span_ssbo);

    // Uniforms.

    glProgramUniform3fv(fr.program,
        fr.light_culling_domain_lower_bounds_location,
        1, data(lower_bound(lc.domain)));
    glProgramUniform3fv(fr.program,
        fr.light_culling_domain_upper_bounds_location,
        1, data(upper_bound(lc.domain)));
    glProgramUniform3iv(fr.program,
        fr.light_culling_resolution_location,
        1, data(lc.resolution));
}

inline
void upload(const ForwardRenderer& fr, const LightGroup& lg) {
    // SSBOs.

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        fr.light_group_light_properties_buffer_binding,
        lg.light_properties_ssbo);
}

inline
void upload(const ForwardRenderer& fr, const MaterialGroup& mg) {
    // Samplers.

    glBindSampler(
        fr.albedo_texture_array_unit,
        mg.albedo_sampler);

    // SSBOs.

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        fr.material_properties_buffer_binding,
        mg.material_properties_ssbo);

    // Textures.

    glBindTextureUnit(
        fr.albedo_texture_array_unit,
        mg.albedo_texture_array);

    // Uniforms.

    glProgramUniform1i(fr.program,
        fr.albedo_texture_array_location,
        fr.albedo_texture_array_unit);
}

inline
void upload(const ForwardRenderer& fr, const ObjectGroupData& ogd) {
    // SSBOs.

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        fr.material_triangle_material_id_buffer_binding,
        ogd.triangle_material_id_ssbo);
}

inline
void upload(const ForwardRenderer& fr, const ObjectGroupTopology& ogt) {
    // SSBOs.

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        fr.object_draw_indirect_buffer_binding,
        ogt.draw_command_buffer);
}
