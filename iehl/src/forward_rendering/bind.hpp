#pragma once

#include "command/all.hpp"
#include "material/all.hpp"
#include "forward_renderer.hpp"

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
