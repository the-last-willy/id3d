#pragma once

#include "adjacent_triangle_count.hpp"
#include "index.hpp"
#include "mesh.hpp"
#include "proxy.hpp"
#include "topology.hpp"

namespace face_vertex {

inline
TriangleProxy adjacent_triangle(TriangleProxy tp, uint32_t i) {
    return proxy(
        mesh(tp),
        TriangleIndex(topology(tp)->triangles[i % adjacent_triangle_count(tp)]));
}

}
