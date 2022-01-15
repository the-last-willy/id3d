#pragma once

#include "command/all.hpp"
#include "scene/object/all.hpp"
#include "occlusion_culler.hpp"

inline
void bind_for_culling(const OcclusionCuller& oc) {
    glUseProgram(oc.cull_program);

    // Textures.

    glProgramUniform1i(oc.cull_program,
        oc.cull_hzb_uniform_location,
        oc.cull_hzb_texture_unit);
    glBindTextureUnit(
        oc.cull_hzb_texture_unit,
        oc.depth_texture);
}

inline
void bind_for_culling(const OcclusionCuller& oc,
    const CommandGroup& input,
    const CommandGroup& output)
{
    // Shader storage buffers.

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        oc.cull_input_draw_commands_buffer_binding,
        input.commands_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        oc.cull_output_draw_command_count_buffer_binding,
        output.count_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        oc.cull_output_draw_commands_buffer_binding,
        output.commands_buffer);
}

inline
void bind_for_culling(const OcclusionCuller& oc,
    const ObjectGroupData& ogd)
{
    // Shader storage buffers.

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        oc.cull_world_object_bounds_buffer_binding,
        ogd.object_bounds_ssbo);
}

inline
void bind_for_drawing(const OcclusionCuller& oc) {
    glBindFramebuffer(GL_FRAMEBUFFER, oc.depth_fbo);
    glUseProgram(oc.draw_program);
}
