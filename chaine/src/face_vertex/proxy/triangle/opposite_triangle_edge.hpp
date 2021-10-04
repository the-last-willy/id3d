#pragma once

#include "adjacent_triangle.hpp"
#include "proxy.hpp"
#include "vertex.hpp"

#include "face_vertex/index/vertex.hpp"
#include "face_vertex/proxy/triangle_edge/proxy.hpp"

#include <stdexcept>

namespace face_vertex {

inline
TriangleEdgeProxy opposite_triangle_edge(TriangleProxy tp, VertexIndex vi) {
    for(uint32_t i = 0; i < 3; ++i) {
        if(index(vertex(tp, i)) == vi) {
            return TriangleEdgeProxy(
                mesh(tp),
                TriangleEdgeIndex(tp, adjacent_triangle(tp, i)));
        }
    }
    throw std::logic_error("Not a triangle vertex.");
}

}
