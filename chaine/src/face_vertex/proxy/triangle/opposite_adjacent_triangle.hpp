#pragma once

#include "adjacent_triangle.hpp"
#include "mesh.hpp"
#include "proxy.hpp"
#include "vertex_count.hpp"

#include "face_vertex/proxy/vertex/proxy.hpp"

#include <stdexcept>

namespace face_vertex {

inline
TriangleProxy opposite_adjacent_triangle(TriangleProxy tp, VertexIndex vi) {
    auto&& to = topology(tp);
    for(uint32_t i = 0; i < vertex_count(tp); ++i) {
        if(to.vertices[i] == vi) {
            return adjacent_triangle(tp, i);
        }
    }
    throw std::logic_error("Not a triangle vertex.");
}

}
