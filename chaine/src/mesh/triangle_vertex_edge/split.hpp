#pragma once

#include "incident_triangle.hpp"
#include "opposite_triangle.hpp"

namespace face_vertex {

inline
auto split(TriangleVertexEdgeProxy tvep) {
    auto t0 = incident_triangle(tvep);
    auto t1 = opposite_triangle(tvep);

    auto v = vertex(t0, relative_index(t0, t1) + 2);

    auto at0 = adjacent_triangle(t0, relative_index(t0, t1) + 1);
    auto at1 = adjacent_triangle(t1, relative_index(t1, t0) + 1);
    
    
}

}
