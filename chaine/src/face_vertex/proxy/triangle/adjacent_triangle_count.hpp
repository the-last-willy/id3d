#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
TriangleIndex adjacent_triangle_count(TriangleProxy) {
    return TriangleIndex(3);
}

}
