#pragma once

#include "proxy.hpp"
#include "vertex_count.hpp"
#include "vertex.hpp"

#include <stdexcept>

namespace face_vertex {

inline
VertexProxy vertex_after(TriangleProxy tp, VertexIndex vi) {
    for(uint32_t i = 0; i < vertex_count(tp); ++i) {
        if(index(vertex(tp, i)) == vi) {
            return vertex(tp, i + 1);
        }
    }
    throw std::logic_error("Not a triangle vertex.");
}

}
