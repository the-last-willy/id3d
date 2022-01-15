#pragma once

#include "command/all.hpp"
#include "scene/object/all.hpp"
#include "frustum_culler.hpp"

inline
void bind(const FrustumCuller& fc) {
    glUseProgram(fc.program);
}

inline
void bind(const FrustumCuller& fc,
    const CommandGroup& input,
    const CommandGroup& output)
{
    // Shader storage buffers.

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        fc.input_draw_commands_buffer_binding,
        input.commands_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        fc.output_draw_command_count_buffer_binding,
        output.count_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        fc.output_draw_commands_buffer_binding,
        output.commands_buffer);
}

inline
void bind(const FrustumCuller& fc,
    const ObjectGroupData& ogd)
{
    // Shader storage buffers.

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        fc.object_world_bounds_buffer_binding,
        ogd.object_bounds_ssbo);
}
