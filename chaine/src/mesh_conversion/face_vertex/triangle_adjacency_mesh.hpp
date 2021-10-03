#pragma once

#include "face_vertex/all.hpp"

#include <agl/all.hpp>
#include <agl/engine/mesh.hpp>

#include <range/v3/view/enumerate.hpp>

inline
eng::Mesh triangle_adjacency_mesh(face_vertex::Mesh& m) {
    auto mesh = eng::Mesh();
    auto& primitive = *mesh.primitives.emplace_back(
        std::make_shared<eng::Primitive>());
    {
        primitive.draw_mode = agl::DrawMode::lines;
        primitive.draw_type = agl::DrawType::unsigned_int;
    }
    {
        auto positions = std::vector<agl::Vec3>();
        for(auto&& t : triangles(m)) {
            auto at0 = adjacent_triangle(t, 0);
            if(not is_ghost(at0)) {
                positions.push_back(barycenter(t));
                positions.push_back(barycenter(at0));
            }
            auto at1 = adjacent_triangle(t, 1);
            if(not is_ghost(at1)) {
                positions.push_back(barycenter(t));
                positions.push_back(barycenter(at1));
            }
            auto at2 = adjacent_triangle(t, 2);
            if(not is_ghost(at2)) {
                positions.push_back(barycenter(t));
                positions.push_back(barycenter(at2));
            }
        }
        if(not empty(positions)) {
            primitive.attributes["POSITION"]
            = agl::engine::accessor(std::span(positions));
        }
        primitive.primitive_count
        = agl::Count<GLsizei>(static_cast<GLsizei>(size(positions)));
    }
    return mesh;
}
