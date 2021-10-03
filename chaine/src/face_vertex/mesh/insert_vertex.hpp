#pragma once

#include "any_ghost_vertex.hpp"
#include "mesh.hpp"
#include "split.hpp"
#include "triangles.hpp"

#include "face_vertex/proxy/triangle/relative_index.hpp"
#include "face_vertex/proxy/vertex/adjacent_triangles.hpp"
#include "face_vertex/proxy/vertex/position.hpp"
#include "face_vertex/proxy/vertex/proxy.hpp"

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
    for(auto&& at : adjacent_triangles(gv)) {
        auto ri = relative_index(at, gv);
        auto v0 = vertex(at, ri + 1);
        auto v1 = vertex(at, ri + 2);
        auto c = cross(pos - position(v0), position(v1) - pos)[2];
        if(c < 0.f) {
            auto s = split(at);
            position(s) = pos;
            return;
        }
    }
}

}
