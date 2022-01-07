#pragma once

#include "opengl.hpp"

#include <agl/engine/all.hpp>

inline
agl::Mat4 wire_quad_model_to_clip(
    GLfloat x0, GLfloat x1,
    GLfloat y0, GLfloat y1,
    GLfloat z)
{
    auto e00 = (x1 - x0) / 2.f;
    auto e11 = (y1 - y0) / 2.f;
    auto e30 = (x1 + x0) / 2.f;
    auto e31 = (y1 + y0) / 2.f;
    auto e32 = z;
    return agl::mat4(
        e00, 0.f, 0.f, 0.f,
        0.f, e11, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f,
        e30, e31, e32, 1.f);
}
