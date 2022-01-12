#pragma once

#include "opengl.hpp"

struct SolidBoxTopology {
    gl::Buffer element_buffer;

    GLsizei draw_count = 3 * 6 * 2;
    GLint draw_first = 0;
    GLenum draw_mode = GL_TRIANGLES;
    GLenum draw_type = GL_UNSIGNED_INT;

    SolidBoxTopology() {
        gl::NamedBufferStorage(element_buffer, {
            // -X
            // For some reason, the back face is wrong with that.
            // It should be that, but it's not.
            // Temporary fix, investigate it.
            // 0, 1, 2,
            // 1, 3, 2,
            0, 2, 1,
            1, 2, 3,
            // -Y
            4, 6, 5,
            5, 6, 7,
            // -Z
            8, 9, 10,
            9, 11, 10,
            // +Z
            12, 14, 13,
            13, 14, 15,
            // +Y
            16, 17, 18,
            17, 19, 18,
            // +X
            20, 22, 21,
            21, 22, 23
            });
    }
};
