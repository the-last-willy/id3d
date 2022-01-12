#pragma once

#include "command_group.hpp"

inline
void copy_cpu_to_gpu(CommandGroup& cg) {
    auto count = GLuint(size(cg.commands));
    glNamedBufferSubData(cg.count_buffer,
        0, sizeof(GLuint), &count);
    if(cg.commands.capacity() > 0) {
        cg.commands_buffer = gl::Buffer();
        gl::NamedBufferStorage(cg.commands_buffer,
            GLsizei(cg.commands.capacity()) * sizeof(gl::DrawElementsIndirectCommand),
            GL_DYNAMIC_STORAGE_BIT);
        glNamedBufferSubData(cg.commands_buffer, 0,
            size(cg) * sizeof(gl::DrawElementsIndirectCommand),
            data(cg.commands));
    }
}
