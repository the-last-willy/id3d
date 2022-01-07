#pragma once

#include "opengl.hpp"

#include <array>

struct WireQuadVertexAttributes {
    using Position = std::array<GLfloat, 3>;

    gl::Buffer position_buffer;

    WireQuadVertexAttributes() {
        gl::NamedBufferStorage(position_buffer, {
            Position{-1.f, -1.f, 0.f},
            Position{-1.f,  1.f, 0.f},
            Position{ 1.f, -1.f, 0.f},
            Position{ 1.f,  1.f, 0.f}});
    }
};
