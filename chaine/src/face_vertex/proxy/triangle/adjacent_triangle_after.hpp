#pragma once

#include "adjacent_triangle.hpp"
#include "adjacent_triangle_count.hpp"
#include "mesh.hpp"
#include "proxy.hpp"

#include <stdexcept>

namespace face_vertex {

inline
TriangleProxy adjacent_triangle_after(TriangleProxy tp, TriangleIndex ti) {
    for(uint32_t i = 0; i < adjacent_triangle_count(tp); ++i) {
        if(index(adjacent_triangle(tp, i)) == ti) {
            return proxy(
                mesh(tp),
                TriangleIndex((i + 1) % adjacent_triangle_count(tp)));
        }
    }
    throw std::logic_error("Not an adjacent triangle.");
}

}
