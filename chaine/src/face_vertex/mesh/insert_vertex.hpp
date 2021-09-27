#pragma once

#include "mesh.hpp"
#include "split.hpp"
#include "triangles.hpp"

#include "face_vertex/proxy/vertex/proxy.hpp"

namespace face_vertex {

inline
void insert_vertex(Mesh& m, agl::Vec3 position) {
    for(auto&& t : triangles(m)) {
        if(contains_xy(t, position)) {
            auto s = split(t);
            face_vertex::position(s) = position;
            return;
        }
    }
}

}
