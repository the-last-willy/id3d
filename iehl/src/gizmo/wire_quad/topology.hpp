#pragma once

#include "opengl.hpp"

struct WireQuadTopology {
    gl::Buffer element_buffer;

    GLsizei draw_count = 8;
    GLint draw_first = 0;
    GLenum draw_mode = GL_LINES;
    GLenum draw_type = GL_UNSIGNED_INT;

    WireQuadTopology() {
        gl::NamedBufferStorage(element_buffer,
            {0, 2, 2, 3, 3, 1, 1, 0});
    }
};
