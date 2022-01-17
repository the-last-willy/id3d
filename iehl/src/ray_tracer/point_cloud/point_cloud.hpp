#pragma once

#include "opengl.hpp"

#include <array>

struct PointCloud {
    using Color = std::array<GLfloat, 3>;
    using Normal = std::array<GLfloat, 3>;
    using Position = std::array<GLfloat, 3>;

    gl::VertexArray vao;

    gl::Buffer color_vbo;
    gl::Buffer normal_vbo;
    gl::Buffer position_vbo;

    GLsizei capacity;
    GLsizei size;

    PointCloud(GLsizei capacity = 10'000'000)
        : capacity(capacity)
        , size(0)
    {
        gl::NamedBufferStorage(color_vbo,
            capacity * sizeof(Color), NULL,
            GL_DYNAMIC_STORAGE_BIT);
        gl::NamedBufferStorage(normal_vbo,
            capacity * sizeof(Normal), NULL,
            GL_DYNAMIC_STORAGE_BIT);
        gl::NamedBufferStorage(position_vbo,
            capacity * sizeof(Position), NULL,
            GL_DYNAMIC_STORAGE_BIT);
    }
};
