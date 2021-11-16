#pragma once

#include "incident_triangle.hpp"
#include "opposite_vertex.hpp"
#include "proxy.hpp"

namespace face_vertex {

inline
auto diameter(TriangleVertexEdgeProxy tvep) {
    auto it = incident_triangle(tvep);
    auto ov = opposite_vertex(tvep);
    auto ri = relative_index(it, ov);
    auto v1 = vertex(it, ri + 1);
    auto v2 = vertex(it, ri + 2);
    return length(position(v1) - position(v2));
}

}
