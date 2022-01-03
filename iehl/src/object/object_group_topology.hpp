#pragma once

#include "opengl.hpp"

#include <array>
#include <vector>

struct ObjectGroupTopology {
    // Should the vertex array and the draw commands be in the same struct ?
    // The draw commands and the element buffer are correlated.
    // Perhaps it would be better to swap vertex array element buffers
    // instead of duplicating vertex arrays when only the element buffer vary ?

    std::vector<std::array<GLuint, 3>> triangle_indices;
    gl::Buffer element_buffer;

    // Should be encapsulated ?
    // It can be either on CPU or GPU side.
    // Tricky, think more about it.

    // Count and commands could be in the same buffer ?
    // For MultiDrawElementCount ?

    // Use persistent mapping ?

    std::vector<gl::DrawElementsIndirectCommand> draw_commands;
    gl::Buffer draw_command_buffer;

    GLsizei draw_count = 0;
    // gl::Buffer draw_count_buffer;
};
