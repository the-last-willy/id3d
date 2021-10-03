#pragma once

#include "mesh.hpp"
#include "vertices.hpp"

#include "face_vertex/proxy/vertex/is_ghost.hpp"

#include <stdexcept>

namespace face_vertex {

inline
VertexProxy any_ghost_vertex(Mesh& m) {
    for(uint32_t i = 0; i < vertex_count(m); ++i) {
        auto v = proxy(m, VertexIndex(i));
        if(is_ghost(v)) {
            return v;
        }
    }
    throw std::logic_error("No ghost vertex.");
}

}
