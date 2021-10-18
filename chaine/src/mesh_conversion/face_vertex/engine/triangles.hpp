#pragma once

#include "face_vertex/all.hpp"

#include <agl/engine/all.hpp>

#include <range/v3/view/enumerate.hpp>

#include <vector>

namespace chaine {

inline
auto triangles_mesh(face_vertex::Mesh& m) {
    auto primitive_ptr = std::make_shared<eng::Primitive>();
    {
        auto indices = std::vector<uint32_t>();
        for(auto&& t : triangles(m)) {
            indices.push_back(topology(t).vertices[0]);
            indices.push_back(topology(t).vertices[1]);
            indices.push_back(topology(t).vertices[2]);
        }
        primitive_ptr->indices = agl::engine::accessor(std::span(indices));
    }
    {
        auto& primitive = *primitive_ptr;
        primitive.attributes["COLOR"] = agl::engine::accessor(
            std::span(m.geometry.vertex_colors));
        primitive.attributes["NORMAL"] = agl::engine::accessor(
            std::span(m.geometry.vertex_normals));
        primitive.attributes["POSITION"] = agl::engine::accessor(
            std::span(m.geometry.vertex_positions));
        primitive.draw_type = agl::DrawType::unsigned_int;
        // primitive.indices = std::move(index_accessor);
        primitive.primitive_count = agl::Count<GLsizei>(GLsizei(3 * triangle_count(m)));
    }
    return eng::Mesh({std::move(primitive_ptr)});
}

}
