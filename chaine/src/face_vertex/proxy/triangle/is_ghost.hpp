#pragma once

#include "proxy.hpp"
#include "topology.hpp"
#include "vertices.hpp"

#include "face_vertex/proxy/vertex/is_ghost.hpp"

namespace face_vertex {

inline
auto is_ghost(TriangleProxy tp) {
    for(auto&& v : vertices(tp)) {
        if(is_ghost(v)) {
            return true;
        }
    }
    return false;
}

}
