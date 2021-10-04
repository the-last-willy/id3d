#pragma once

#include "any_ghost_vertex.hpp"
#include "mesh.hpp"
#include "split.hpp"
#include "triangles.hpp"

#include "face_vertex/proxy/triangle/opposite_triangle_edge.hpp"
#include "face_vertex/proxy/triangle/relative_index.hpp"
#include "face_vertex/proxy/vertex/adjacent_triangles.hpp"
#include "face_vertex/proxy/vertex/position.hpp"
#include "face_vertex/proxy/vertex/proxy.hpp"

#include "predicate/all.hpp"

namespace face_vertex {

inline
void insert_vertex(Mesh& m, agl::Vec3 pos) {
    // Interior.
    for(auto&& t : triangles(m)) {
        if(contains_xy(t, pos)) {
            auto s = split(t);
            face_vertex::position(s) = pos;
            return;
        }
    }
    // Exterior.
    auto&& gv = any_ghost_vertex(m);

    TriangleProxy t1;
    VertexProxy v1;

    TriangleProxy t2;
    VertexProxy v2;

    for(auto&& at : adjacent_triangles(gv)) {
        auto ri = relative_index(at, gv);
        v1 = vertex(at, ri + 1);
        v2 = vertex(at, ri + 2);
        auto p1 = position(v1).xy();
        auto p2 = position(v2).xy();
        if(orientation(p1, p2, pos.xy()) > 0.f) {
            t1 = opposite_triangle(at, v1);
            t2 = opposite_triangle(at, v2);

            auto s = split(at);
            position(s) = pos;
            break;;
        }
    }

    // Flip way 1.
    while(true) {
        auto ri = relative_index(t1, gv);
        v1 = vertex(t1, ri + 1);
        v12 = vertex(t1, ri + 2);
        auto p1 = position(v1).xy();
        auto p2 = position(v12).xy();
        if(orientation(p1, p2, pos.xy()) > 0.f) {
            flip(opposite_triangle_edge(t1, v12));
            break;
        } else {
            break;
        }
    }

    // // Flip way 2.
    // while(true) {

    // }
}

}