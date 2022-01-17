#pragma once

#include "command/all.hpp"
#include "scene/lighting/all.hpp"
#include "scene/material/all.hpp"
#include "scene/object/all.hpp"
#include "forward_renderer.hpp"

inline
void bind_for_depth(const ForwardRenderer& fr) {
    glUseProgram(fr.depth_program);
}

inline
void bind_for_depth(const ForwardRenderer& fr,
    const CommandGroup& cg)
{
    // SSBOs.

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        fr.depth_object_draw_indirect_buffer,
        cg.commands_buffer);
}

inline
void bind_for_depth(const ForwardRenderer& fr, 
    const MaterialGroup& mg)
{
    // Samplers.

    glBindSampler(
        fr.depth_material_albedo_opacity_tu,
        mg.albedo_sampler);

    // Textures.

    glBindTextureUnit(
        fr.depth_material_albedo_opacity_tu,
        mg.albedo_texture_array);

    // Uniforms.

    glProgramUniform1i(fr.depth_program,
        fr.depth_material_albedo_opacity_ul,
        fr.depth_material_albedo_opacity_tu);
}

inline
void bind_for_depth(const ForwardRenderer& fr, 
    const ObjectGroupData& ogd)
{
    // SSBOs.

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        fr.depth_material_triangle_material_id_buffer,
        ogd.triangle_material_id_ssbo);
}

inline
void bind(const ForwardRenderer& fr) {
    glUseProgram(fr.program);
}

inline
void bind(const ForwardRenderer& fr,
    const CommandGroup& cg)
{
    // SSBOs.

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        fr.object_draw_indirect_buffer_binding,
        cg.commands_buffer);
}

inline
void bind(const ForwardRenderer& fr, 
    const MaterialGroup& mg)
{
    // Samplers.

    glBindSampler(
        fr.albedo_texture_array_unit,
        mg.albedo_sampler);
    glBindSampler(
        fr.ao_roughness_metallic_texture_unit,
        mg.ao_roughness_metallic_sampler);

    // SSBOs.

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        fr.material_properties_buffer_binding,
        mg.material_properties_ssbo);

    // Textures.

    glBindTextureUnit(
        fr.albedo_texture_array_unit,
        mg.albedo_texture_array);
    glBindTextureUnit(
        fr.ao_roughness_metallic_texture_unit,
        mg.ao_roughness_metallic_texture_array);

    // Uniforms.

    glProgramUniform1i(fr.program,
        fr.albedo_texture_array_location,
        fr.albedo_texture_array_unit);
}

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
void upload(const ForwardRenderer& fr, const ObjectGroupData& ogd) {
    // SSBOs.

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        fr.material_triangle_material_id_buffer_binding,
        ogd.triangle_material_id_ssbo);
}
