// #pragma once

// #include <face_vertex/all.hpp>

// #include <range/v3/view/enumerate.hpp>

// inline
// auto edges_mesh(face_vertex::Mesh& m) {
//     auto primitive = std::make_shared<eng::Primitive>();
//     {
//         primitive->draw_mode = agl::DrawMode::lines;
//         primitive->draw_type = agl::DrawType::unsigned_int;
//     }
//     {
//         auto positions = std::vector<agl::Vec3>();
//         for(auto&& t : triangles(m)) {
//             if(not is_ghost(t)) {
//                 positions.push_back(position(vertex(t, 0)));
//                 positions.push_back(position(vertex(t, 1)));

//                 positions.push_back(position(vertex(t, 1)));
//                 positions.push_back(position(vertex(t, 2)));

//                 positions.push_back(position(vertex(t, 2)));
//                 positions.push_back(position(vertex(t, 0)));
//             }
//         }

//         primitive->attributes["POSITION"]
//         = agl::engine::accessor(std::span(positions));
//         primitive->primitive_count
//         = agl::Count<GLsizei>(static_cast<GLsizei>(size(positions)));
//     }
//     return eng::Mesh({std::move(primitive)});
// }

#pragma once

#include <face_vertex/all.hpp>

#include <range/v3/view/enumerate.hpp>

inline
auto edges_mesh(face_vertex::Mesh& m) {
    auto primitive = std::make_shared<eng::Primitive>();
    {
        primitive->draw_mode = agl::DrawMode::lines;
        primitive->draw_type = agl::DrawType::unsigned_int;
    }
    {
        auto indices = std::vector<uint32_t>();
        for(auto&& t : triangles(m)) {
            indices.push_back(index(vertex(t, 0)));
            indices.push_back(index(vertex(t, 1)));

            indices.push_back(index(vertex(t, 1)));
            indices.push_back(index(vertex(t, 2)));

            indices.push_back(index(vertex(t, 2)));
            indices.push_back(index(vertex(t, 0)));
        }

        primitive->indices = agl::engine::accessor(std::span(indices));

        primitive->attributes["POSITION"]
        = agl::engine::accessor(std::span(m.geometry.vertex_positions));
        primitive->primitive_count
        = agl::Count<GLsizei>(static_cast<GLsizei>(size(indices)));
    }
    return eng::Mesh({std::move(primitive)});
}
