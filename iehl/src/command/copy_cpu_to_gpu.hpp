#pragma once

#include "command_group.hpp"

inline
void copy_cpu_to_gpu(CommandGroup& cg) {
    cg.count_buffer = gl::Buffer();
    gl::NamedBufferStorage(cg.count_buffer,
        { GLsizei(size(cg)) });
    cg.commands_buffer = gl::Buffer();
    gl::NamedBufferStorage(cg.commands_buffer,
        cg.commands);
}
