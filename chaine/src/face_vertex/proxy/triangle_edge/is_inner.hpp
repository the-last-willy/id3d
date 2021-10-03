#pragma once

#include "adjacent_triangle.hpp"
#include "proxy.hpp"

#include "face_vertex/proxy/triangle/is_ghost.hpp"

namespace face_vertex {

inline
bool is_inner(TriangleEdgeProxy tep) {
    return not is_ghost(adjacent_triangle(tep, 0)) 
    and not is_ghost(adjacent_triangle(tep, 1));
}

}
