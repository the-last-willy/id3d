#pragma once

#include "adjacent_triangle.hpp"
#include "proxy.hpp"

#include "mesh/triangle/adjacent_triangle.hpp"
#include "mesh/triangle/vertex.hpp"
#include "mesh/triangle/vertex_count.hpp"

namespace face_vertex {

inline
bool is_locally_delaunay(TriangleEdgeProxy tep) {
    auto at0 = adjacent_triangle(tep, 0);
    auto p0 = position(vertex(at0, 0));
    auto p1 = position(vertex(at0, 1));
    auto p2 = position(vertex(at0, 2));
    auto at1 = adjacent_triangle(tep, 1);
    agl::Vec3 p3;
    for(uint32_t i = 0; i < vertex_count(at1); ++i) {
        if(index(adjacent_triangle(at1, i)) == index(at0)) {
            p3 = position(vertex(at1, i));
        }
    }
    p0[2] = 0.f;
    p1 -= p0;
    p1[2] = p1[0] * p1[0] + p1[1] * p1[1];
    p2 -= p0;
    p2[2] = p2[0] * p2[0] + p2[1] * p2[1];
    p3 -= p0;
    p3[2] = p3[0] * p3[0] + p3[1] * p3[1];
    return dot(cross(p1 - p0, p2 - p0), p3 - p0) > 0.f;
}

}
