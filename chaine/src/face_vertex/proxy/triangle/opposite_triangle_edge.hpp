#pragma once

#include "opposite_triangle.hpp"
#include "proxy.hpp"

#include "face_vertex/proxy/triangle_edge/proxy.hpp"

namespace face_vertex {

inline
TriangleEdgeProxy opposite_triangle_edge(TriangleProxy tp, VertexIndex vi) {
    auto ot = opposite_triangle(tp, vi);
    return proxy(
        mesh(tp),
        TriangleEdgeIndex(std::array{index(tp), index(ot)}));
}

}
