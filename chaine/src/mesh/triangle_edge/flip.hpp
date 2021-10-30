#pragma once

#include "adjacent_triangle.hpp"
#include "proxy.hpp"

#include "mesh/triangle/opposite_vertex.hpp"
#include "mesh/triangle/substitute_adjacent_triangle.hpp"

namespace face_vertex {

inline
void flip(TriangleEdgeProxy tep) {
    auto t0 = adjacent_triangle(tep, 0);
    auto t1 = adjacent_triangle(tep, 1);

    auto t0_t1_i = relative_index(t0, t1);
    auto t0_at0_i = (t0_t1_i + 1) % 3;
    auto t1_t0_i = relative_index(t1, t0);
    auto t1_at1_i = (t1_t0_i + 1) % 3;

    auto at0 = adjacent_triangle(t0, t0_at0_i);
    auto at1 = adjacent_triangle(t1, t1_at1_i);

    auto tt0 = topology(t0);
    auto tt1 = topology(t1);

    {
        tt0->vertices[(t0_t1_i + 2) % 3] = tt1->vertices[t1_t0_i];
        tt1->vertices[(t1_t0_i + 2) % 3] = tt0->vertices[t0_t1_i];
        topology(vertex(t0, t0_at0_i))->triangle = t0;
        topology(vertex(t1, t1_at1_i))->triangle = t1;

    }

    topology(t0)->triangles[t0_at0_i] = index(t1);
    topology(t1)->triangles[t1_at1_i] = index(t0);

    if(index(at0) != index(t0)) {
        topology(t1)->triangles[t1_t0_i] = index(at0);
        substitute_adjacent_triangle(at0, t0, t1);
    }
    if(index(at1) != index(t1)) {
        topology(t0)->triangles[t0_t1_i] = index(at1);
        substitute_adjacent_triangle(at1, t1, t0);
    }
}

}
