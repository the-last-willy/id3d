#pragma once

#include "command/all.hpp"
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

    CommandGroup commands;
};
