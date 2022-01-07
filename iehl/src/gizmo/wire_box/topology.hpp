#pragma once

#include "opengl.hpp"

struct WireBoxTopology {
    gl::Buffer element_buffer;

    GLsizei draw_count = 24;
    GLint draw_first = 0;
    GLenum draw_mode = GL_LINES;
    GLenum draw_type = GL_UNSIGNED_INT;

    WireBoxTopology() {
        gl::NamedBufferStorage(element_buffer, {
            0, 1,
            0, 2,
            0, 4,
            1, 3,
            1, 5,
            2, 3,
            2, 6,
            3, 7,
            4, 5,
            4, 6,
            5, 7,
            6, 7});
    }
};
