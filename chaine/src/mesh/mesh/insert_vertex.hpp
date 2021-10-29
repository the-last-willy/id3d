#pragma once

#include "mesh.hpp"
#include "split.hpp"
#include "triangles.hpp"

#include "mesh/triangle/opposite_triangle_edge.hpp"
#include "mesh/triangle/relative_index.hpp"
#include "mesh/vertex/adjacent_triangles.hpp"
#include "mesh/vertex/position.hpp"
#include "mesh/vertex/proxy.hpp"

#include "predicate/all.hpp"

// namespace face_vertex {

// inline
// void insert_vertex(Mesh& m, agl::Vec3 pos) {
//     // Interior.
//     for(auto&& t : triangles(m)) {
//         if(contains_xy(t, pos)) {
//             auto s = split(t);
//             face_vertex::position(s) = pos;
//             return;
//         }
//     }
// }

// }