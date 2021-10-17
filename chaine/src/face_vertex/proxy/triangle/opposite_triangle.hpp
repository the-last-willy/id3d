#pragma once

#include "adjacent_triangle.hpp"
#include "proxy.hpp"
#include "relative_index.hpp"

namespace face_vertex {

inline
TriangleProxy opposite_triangle(TriangleProxy tp, VertexIndex vi) {
    return adjacent_triangle(tp, relative_index(tp, vi));
}

}
