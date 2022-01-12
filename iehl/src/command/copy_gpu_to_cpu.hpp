#pragma once

#include "command_group.hpp"

#include <iostream>

inline
void copy_gpu_to_cpu(CommandGroup& cg) {
    GLuint count;
    glGetNamedBufferSubData(cg.count_buffer, 0,
        sizeof(GLuint), &count);
    // std::cout << "gpu_to_cpu " << count << std::endl;
    cg.commands.resize(count);
    gl::GetNamedBufferSubData(cg.commands_buffer, 0,
        cg.commands);
}
