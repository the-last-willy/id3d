#pragma once

#include "incident_triangle.hpp"
#include "opposite_triangle.hpp"

namespace face_vertex {

inline
auto split(TriangleVertexEdgeProxy tvep) {
    auto&& m = mesh(tvep);

    auto t0 = incident_triangle(tvep);
    auto t1 = opposite_triangle(tvep);

    auto t0_t1_i = relative_index(t0, t1);
    auto t0_at0_i = (t0_t1_i + 2) % 3;
    auto t0_v0_i = (t0_t1_i + 1) % 3;
    auto t1_t0_i = relative_index(t1, t0);
    auto t1_at1_i = (t1_t0_i + 1) % 3;
    auto t1_v0_i = (t1_t0_i + 2) % 3;

    auto v0 = vertex(t0, t0_t1_i + 1);

    auto at0 = adjacent_triangle(t0, t0_at0_i);
    auto at1 = adjacent_triangle(t1, t1_at1_i);

    auto at0_t0_i = relative_index(at0, t0);
    auto at1_t1_i = relative_index(at1, t1);

    auto v1 = create_vertex(m);

    auto nt0 = create_triangle(m);

    {
        topology(t0)->vertices[t0_v0_i] = index(v1);
    }
    {
        topology(v0)->triangle = index(nt0);
        topology(v1)->triangle = index(nt0);
    }
    {
        topology(nt0)->vertices[0] = index(v1);
        topology(nt0)->vertices[1] = index(vertex(t0, t0_t1_i));
        topology(nt0)->vertices[2] = index(v0);

        // 1, t0 <-> nt0
        topology(t0)->triangles[t0_at0_i] = index(nt0);
        topology(nt0)->triangles[2] = index(t0);
        
        if(index(at0) != index(t0)) {
            // 2, at0 <-> nt0
            topology(at0)->triangles[at0_t0_i] = index(nt0);
            topology(nt0)->triangles[0] = index(at0);
        }
    }
    if(index(t1) != index(t0)) {
        {
            topology(t1)->vertices[t1_v0_i] = index(v1);
        }
        auto nt1 = create_triangle(m);
        topology(nt1)->vertices[0] = index(v1);
        topology(nt1)->vertices[1] = index(v0);
        topology(nt1)->vertices[2] = index(vertex(t1, t1_t0_i));

        // 3, t1 <-> nt1
        topology(t1)->triangles[t1_at1_i] = index(nt1);
        topology(nt1)->triangles[1] = index(t1);

        // 4, nt0 <-> nt1
        topology(nt0)->triangles[1] = index(nt1);
        topology(nt1)->triangles[2] = index(nt0);
        
        if(index(at1) != index(t1)) {
            // 5, at1 <-> nt1
            topology(at1)->triangles[at1_t1_i] = index(nt1);
            topology(nt1)->triangles[0] = index(at1);
        }
    }

    return v1;
}

}
