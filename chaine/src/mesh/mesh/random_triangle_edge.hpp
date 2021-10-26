#pragma once

#include "mesh.hpp"
#include "triangle.hpp"

#include "mesh/triangle_edge/proxy.hpp"

#include <random>
#include <stdexcept>

namespace face_vertex {

template<typename Random>
TriangleEdgeProxy random_triangle_edge(Mesh& m, Random& r) {
    auto tc = uint32_t(size(topology(m).triangles));
    auto triangle_d = std::uniform_int_distribution<uint32_t>(0, tc - 1);
    auto t = triangle(m, triangle_d(r));
    while(not is_valid(t)) {
        t = triangle(m, triangle_d(r));
    }
    auto opposite_d = std::uniform_int_distribution<uint32_t>(
        0, adjacent_triangle_count(t) - 1);
    auto ot = adjacent_triangle(t, opposite_d(r));
    while(not is_valid(ot)) {
        ot = adjacent_triangle(t, opposite_d(r));
    }
    return proxy(m, TriangleEdgeIndex(std::array{index(t), index(ot)}));
}

}
