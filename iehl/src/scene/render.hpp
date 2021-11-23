#pragma once

#include "scene.hpp"

#include <agl/engine/all.hpp>

inline
void render(Scene& s) {
    bind(s.program);
    bind(s.vertex_array);
    agl::draw_elements(
        agl::DrawMode::triangles,
        agl::Count<GLsizei>(GLsizei(3 * size(s.triangle_indices))),
        agl::DrawType::unsigned_int,
        agl::Offset<GLintptr>(0));
    unbind(s.program);
}
