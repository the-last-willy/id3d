#pragma once

#include "adjacent_triangle.hpp"
#include "can_collapse.hpp"
#include "proxy.hpp"

#include "face_vertex/proxy/triangle/adjacent_triangle_after.hpp"
#include "face_vertex/proxy/triangle/invalidate.hpp"
#include "face_vertex/proxy/triangle/opposite_vertex.hpp"
#include "face_vertex/proxy/triangle/string.hpp"
#include "face_vertex/proxy/triangle/substitute_adjacent_triangle.hpp"
#include "face_vertex/proxy/triangle/vertex_after.hpp"
#include "face_vertex/proxy/vertex/invalidate.hpp"
#include "face_vertex/proxy/vertex/proxy.hpp"
#include "face_vertex/proxy/vertex/substitute_by.hpp"

#include <iostream>

namespace face_vertex {

// Don't contract if same vertex blablabla.
// Can collapse fn.

// Returns collapsed vertex.
inline
VertexProxy collapse(const TriangleEdgeProxy& tep) {
    // std::cout << "collpase " << tep.index.value[0] << " " << tep.index.value[1] << " " << can_collapse(tep) << std::endl;


    // std::cout << "BEFORE" << std::endl;

    auto t0 = adjacent_triangle(tep, 0);
    auto t1 = adjacent_triangle(tep, 1);
    
    auto va = vertex_after(t0, opposite_vertex(t0, t1));
    auto vb = vertex_after(t1, opposite_vertex(t1, t0));

    auto t0a = adjacent_triangle_after(t0, t1);
    auto t0b = adjacent_triangle_after(t0, t0a);

    auto t1a = adjacent_triangle_after(t1, t0);
    auto t1b = adjacent_triangle_after(t1, t1a);

    // std::cout << "t0: " << string(t0);
    // std::cout << "t1: " << string(t1);

    // std::cout << "t0a: " << string(t0a);
    // std::cout << "t0b: " << string(t0b);

    // std::cout << "t1a: " << string(t1a);
    // std::cout << "t1b: " << string(t1b);
    
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

    // std::cout << "AFTER" << std::endl;

    // std::cout << "t0: " << string(t0);
    // std::cout << "t1: " << string(t1);

    // std::cout << "t0a: " << string(t0a);
    // std::cout << "t0b: " << string(t0b);

    // std::cout << "t1a: " << string(t1a);
    // std::cout << "t1b: " << string(t1b);
    
    return va;
}

}

