#pragma once

#include "adjacent_triangle_count.hpp"
#include "adjacent_triangle.hpp"
#include "proxy.hpp"
#include "vertex.hpp"
#include "vertex_count.hpp"

#include "face_vertex/index/vertex.hpp"

#include <stdexcept>

namespace face_vertex {

inline
uint32_t relative_index(TriangleProxy tp, TriangleIndex ti) {
    for(uint32_t i = 0; i < adjacent_triangle_count(tp); ++i) {
        if(index(adjacent_triangle(tp, i)) == ti) {
            return i;
        }
    }
    throw std::logic_error("Not an adjacent triangle.");
}

inline
uint32_t relative_index(TriangleProxy tp, VertexIndex vi) {
    for(uint32_t i = 0; i < vertex_count(tp); ++i) {
        if(index(vertex(tp, i)) == vi) {
            return i;
        }
    }
    throw std::logic_error("Not an incident vertex.");
}

}
