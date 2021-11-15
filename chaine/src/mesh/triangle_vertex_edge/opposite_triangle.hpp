#pragma once

#include "triangle/opposite_triangle.hpp"
#include "incident_triangle.hpp"
#include "opposite_vertex.hpp"
#include "proxy.hpp"

namespace face_vertex {

inline
auto opposite_triangle(TriangleVertexEdgeProxy tvep) {
    return opposite_triangle(
        incident_triangle(tvep),
        opposite_vertex(tvep));
}
    
}
