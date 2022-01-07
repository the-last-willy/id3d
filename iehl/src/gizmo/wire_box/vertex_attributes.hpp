#pragma once

#include "opengl.hpp"

#include <array>

struct WireBoxVertexAttributes {
    using Position = std::array<GLfloat, 3>;

    gl::Buffer position_buffer;

    WireBoxVertexAttributes() {
        gl::NamedBufferStorage(position_buffer, {
            Position{-1.f, -1.f, -1.f},
            Position{-1.f, -1.f,  1.f},
            Position{-1.f,  1.f, -1.f},
            Position{-1.f,  1.f,  1.f},
            Position{ 1.f, -1.f, -1.f},
            Position{ 1.f, -1.f,  1.f},
            Position{ 1.f,  1.f, -1.f},
            Position{ 1.f,  1.f,  1.f}});
    }
};
