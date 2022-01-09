#pragma once

#include "command/all.hpp"
#include "forward_renderer.hpp"

inline
void bind(const ForwardRenderer& fr,
    const CommandGroup& cg)
{
    // SSBOs.

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
        fr.object_draw_indirect_buffer_binding,
        cg.commands_buffer);
}
