#pragma once

#include "proxy.hpp"
#include "relative_index.hpp"

namespace face_vertex {

inline
void substitute_adjacent_triangle(
    TriangleProxy tp,
    TriangleIndex before,
    TriangleIndex after)
{
    topology(tp)->triangles[relative_index(tp, before)] = after;
}

}
