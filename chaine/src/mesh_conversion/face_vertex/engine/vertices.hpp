#pragma once

#include "face_vertex/all.hpp"

#include <agl/engine/all.hpp>

#include <range/v3/view/enumerate.hpp>

namespace chaine {

inline
auto vertices_mesh(face_vertex::Mesh& m) {
    auto primitive_ptr = std::make_shared<eng::Primitive>();
    {
        auto& primitive = *primitive_ptr;
        primitive.attributes["COLOR"] = agl::engine::accessor(
            std::span(m.geometry.vertex_colors));
        primitive.attributes["POSITION"] = agl::engine::accessor(
            std::span(m.geometry.vertex_positions));
        primitive.draw_mode = agl::DrawMode::points;
        primitive.draw_type = agl::DrawType::unsigned_int;
        primitive.primitive_count = agl::Count<GLsizei>(GLsizei(vertex_count(m)));
    }

    return eng::Mesh({std::move(primitive_ptr)});
}

}
