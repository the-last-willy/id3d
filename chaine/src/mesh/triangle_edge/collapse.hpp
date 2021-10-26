#pragma once

#include "adjacent_triangle.hpp"
#include "can_collapse.hpp"
#include "proxy.hpp"

#include "mesh/triangle/adjacent_triangle_after.hpp"
#include "mesh/triangle/invalidate.hpp"
#include "mesh/triangle/opposite_vertex.hpp"
#include "mesh/triangle/string.hpp"
#include "mesh/triangle/substitute_adjacent_triangle.hpp"
#include "mesh/triangle/vertex_after.hpp"
#include "mesh/vertex/invalidate.hpp"
#include "mesh/vertex/proxy.hpp"
#include "mesh/vertex/substitute_by.hpp"

#include <iostream>

namespace face_vertex {

// Don't contract if same vertex blablabla.
// Can collapse fn.

// Returns collapsed vertex.
inline
VertexProxy collapse(const TriangleEdgeProxy& tep) {
    std::cout << "collapse " << tep.index.value[0] << " " << tep.index.value[1] << " " << can_collapse(tep) << std::endl;

    auto t0 = adjacent_triangle(tep, 0);
    auto t1 = adjacent_triangle(tep, 1);

    std::cout << "0: " << index(opposite_vertex(t0, t1)) << std::endl;
    std::cout << "1: " << index(opposite_vertex(t1, t0)) << std::endl;
    
    auto va = vertex_after(t0, opposite_vertex(t0, t1));
    auto vb = vertex_after(t1, opposite_vertex(t1, t0));

    auto t0a = adjacent_triangle_after(t0, t1);
    auto t0b = adjacent_triangle_after(t0, t0a);

    auto t1a = adjacent_triangle_after(t1, t0);
    auto t1b = adjacent_triangle_after(t1, t1a);

    std::cout << "BEFORE" << std::endl;

    std::cout << "t0: " << string(t0);
    std::cout << "t1: " << string(t1);

    std::cout << "t0a: " << string(t0a);
    std::cout << "t0b: " << string(t0b);

    std::cout << "t1a: " << string(t1a);
    std::cout << "t1b: " << string(t1b);
    
    { // Remove vb.
        substitute_by(vb, va);
        invalidate(vb);
    }
    { // Sew t0 faces.
        substitute_adjacent_triangle(t0a, t0, t0b);
        substitute_adjacent_triangle(t0b, t0, t0a);
    }
    { // Sew t1 faces.
        substitute_adjacent_triangle(t1b, t1, t1a);
        substitute_adjacent_triangle(t1a, t1, t1b);
    }
    {
        invalidate(t0);
    }
    {
        invalidate(t1);
    }
    {
        topology(va)->triangle = index(t0a);
        // std::cout << "va: " << is_valid(va) << std::endl;
        topology(vertex_after(t0b, va))->triangle = index(t0b);
        // std::cout << "t0b: " << is_valid(vertex_after(t0b, va)) << std::endl;
        topology(vertex_after(t1b, va))->triangle = index(t1b);
        // std::cout << "t1b: " << is_valid(vertex_after(t1b, va)) << std::endl;
    }

    std::cout << "AFTER" << std::endl;

    std::cout << "t0: " << string(t0);
    std::cout << "t1: " << string(t1);

    std::cout << "t0a: " << string(t0a);
    std::cout << "t0b: " << string(t0b);

    std::cout << "t1a: " << string(t1a);
    std::cout << "t1b: " << string(t1b);
    
    return va;
}

}

