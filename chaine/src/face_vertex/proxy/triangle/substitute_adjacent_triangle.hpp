#pragma once

#include "adjacent_triangle.hpp"
#include "adjacent_triangle_count.hpp"
#include "proxy.hpp"

#include <stdexcept>

namespace face_vertex {

inline
void substitute_adjacent_triangle(
    TriangleProxy tp,
    TriangleIndex before,
    TriangleIndex after) {

}
    for(uint32_t i = 0; i < adjacent_triangle_count(tp); ++i) {
        if(adjacent_triangle(tp, i) == before) {
            topology(tp).triangles[i] = after;
        }
    }
    throw std::logic_error("Not an adjacent triangle.");
}
