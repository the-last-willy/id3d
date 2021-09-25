#pragma once

#include "adjacent_triangle.hpp"
#include "proxy.hpp"

#include "face_vertex/proxy/triangle/opposite_vertex.hpp"
#include "face_vertex/proxy/triangle/substitute_adjacent_triangle.hpp"

namespace face_vertex {

inline
void flip(TriangleEdgeProxy tep) {
    auto at0 = adjacent_triangle(tep, 0);
    auto at1 = adjacent_triangle(tep, 1);
    auto ata0 = adjacent_triangle_after(at0, at1);
    auto ata1 = adjacent_triangle_after(at1, at0);
    substitute_adjacent_triangle(at0, at1, ata1);
    substitute_adjacent_triangle(at0, ata0, at1);
    substitute_adjacent_triangle(at1, at0, ata0);
    substitute_adjacent_triangle(at1, ata1, at0);
    substitute_adjacent_triangle(ata0, at0, at1);
    substitute_adjacent_triangle(ata1, at1, at0);
}

}
