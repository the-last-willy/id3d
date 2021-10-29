#pragma once

#include "mesh/all.hpp"
#include "predicate/orientation.hpp"

inline
bool is_delaunay(face_vertex::TriangleEdgeProxy tep) {
    auto at0 = adjacent_triangle(tep, 0);
    auto origin = opposite_vertex(adjacent_triangle(tep, 1), at0);
    return not orientation(
        position(origin),
        position(vertex(at0, 0)),
        position(vertex(at0, 1)),
        position(vertex(at0, 2)));
}
