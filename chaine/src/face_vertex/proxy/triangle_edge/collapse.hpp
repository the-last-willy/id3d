#pragma once

#include "adjacent_triangle.hpp"
#include "proxy.hpp"

#include "face_vertex/proxy/triangle/adjacent_triangle_after.hpp"
#include "face_vertex/proxy/triangle/opposite_vertex.hpp"
#include "face_vertex/proxy/triangle/substitute_adjacent_triangle.hpp"
#include "face_vertex/proxy/triangle/vertex_after.hpp"
#include "face_vertex/proxy/vertex/proxy.hpp"
#include "face_vertex/proxy/vertex/substitute.hpp"

namespace face_vertex {

// Don't contract if same vertex blablabla.
// Can collapse fn.

// Returns collapsed vertex.
inline
VertexProxy collapse(const TriangleEdgeProxy& tep) {
    auto t0 = adjacent_triangle(tep, 0);
    auto t1 = adjacent_triangle(tep, 1);
    { // Sew t0 faces.
        auto t0a = adjacent_triangle_after(t0, t1);
        auto t0b = adjacent_triangle_after(t0, t0a);
        substitute_adjacent_triangle(t0a, t0, t0b);
        substitute_adjacent_triangle(t0b, t0, t0a);
    }
    { // Sew t1 faces.
        auto t1a = adjacent_triangle_after(t1, t0);
        auto t1b = adjacent_triangle_after(t1, t1a);
        substitute_adjacent_triangle(t1b, t1, t1a);
        substitute_adjacent_triangle(t1a, t1, t1b);
    }
    { // Invalidate t0.
        
    }
    { // Invalidate t1.

    }
    auto va = vertex_after(t0, opposite_vertex(t0, t1));
    auto vb = vertex_after(t1, opposite_vertex(t1, t0));
    { // Remove vb.
        substitute(vb, va);
        // Invalidates vb the bad way.
        topology(vb).is_valid = false;
    }
    return va;
}

}

