#pragma once

#include "opengl.hpp"

#include <array>

struct SolidBoxVertexAttributes {
    using Normal = std::array<GLfloat, 3>;
    using Position = std::array<GLfloat, 3>;

    gl::Buffer normal_buffer;
    gl::Buffer position_buffer;

    SolidBoxVertexAttributes() {
        gl::NamedBufferStorage(normal_buffer, {
            // -Z
            Normal{-1.f,  0.f,  0.f},
            Normal{-1.f,  0.f,  0.f},
            Normal{-1.f,  0.f,  0.f},
            Normal{-1.f,  0.f,  0.f},
            // -Y
            Normal{ 0.f, -1.f,  0.f},
            Normal{ 0.f, -1.f,  0.f},
            Normal{ 0.f, -1.f,  0.f},
            Normal{ 0.f, -1.f,  0.f}, 
            // -X
            Normal{ 0.f,  0.f, -1.f},
            Normal{ 0.f,  0.f, -1.f},
            Normal{ 0.f,  0.f, -1.f},
            Normal{ 0.f,  0.f, -1.f},
            // +X
            Normal{ 0.f,  0.f, +1.f},
            Normal{ 0.f,  0.f, +1.f},
            Normal{ 0.f,  0.f, +1.f},
            Normal{ 0.f,  0.f, +1.f},
            // -Y
            Normal{ 0.f, +1.f,  0.f},
            Normal{ 0.f, +1.f,  0.f},
            Normal{ 0.f, +1.f,  0.f},
            Normal{ 0.f, +1.f,  0.f},
            // +X
            Normal{+1.f,  0.f,  0.f},
            Normal{+1.f,  0.f,  0.f},
            Normal{+1.f,  0.f,  0.f},
            Normal{+1.f,  0.f,  0.f}});

        gl::NamedBufferStorage(position_buffer, {
            // -X
            Position{-1.f, -1.f, -1.f},
            Position{-1.f, -1.f, +1.f},
            Position{-1.f, +1.f, -1.f},
            Position{-1.f, +1.f, +1.f},
            // -Y
            Position{-1.f, -1.f, -1.f},
            Position{+1.f, -1.f, -1.f},
            Position{-1.f, -1.f, +1.f},
            Position{+1.f, -1.f, +1.f},
            // -Z
            Position{-1.f, -1.f, -1.f},
            Position{+1.f, -1.f, -1.f},
            Position{-1.f, +1.f, -1.f},
            Position{+1.f, +1.f, -1.f},
            // +Z
            Position{-1.f, -1.f, +1.f},
            Position{+1.f, -1.f, +1.f},
            Position{-1.f, +1.f, +1.f},
            Position{+1.f, +1.f, +1.f},
            // +Y
            Position{-1.f, +1.f, -1.f},
            Position{+1.f, +1.f, -1.f},
            Position{-1.f, +1.f, +1.f},
            Position{+1.f, +1.f, +1.f},
            // +X
            Position{+1.f, -1.f, -1.f},
            Position{+1.f, +1.f, -1.f},
            Position{+1.f, -1.f, +1.f},
            Position{+1.f, +1.f, +1.f}});
    }
};
