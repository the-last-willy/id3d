#pragma once

#include "adjacent_triangle.hpp"
#include "adjacent_triangle_count.hpp"
#include "proxy.hpp"
#include "vertex.hpp"

#include "face_vertex/proxy/vertex/index.hpp"

#include <stdexcept>

namespace face_vertex {

inline
void substitute_vertex(
    TriangleProxy tp,
    VertexIndex before,
    VertexIndex after)
{
    for(uint32_t i = 0; i < adjacent_triangle_count(tp); ++i) {
        if(index(vertex(tp, i)) == before) {
            topology(tp).vertices[i] = after;
        }
    }
    throw std::logic_error("Not a triangle vertex.");
}

}
