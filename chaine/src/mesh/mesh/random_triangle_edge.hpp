#pragma once

#include "mesh.hpp"
#include "triangle.hpp"

#include "mesh/triangle_edge/proxy.hpp"

#include <random>
#include <stdexcept>

namespace face_vertex {

template<typename Random>
TriangleEdgeProxy random_triangle_edge(Mesh& m, Random& r) {
    auto tc = triangle_count(m);
    auto triangle_d = std::uniform_int_distribution<uint32_t>(0, tc - 1);
    auto t = triangle(m, triangle_d(r));
    while(not is_valid(t)) {
        t = triangle(m, triangle_d(r));
    }
    for(uint32_t i = 0; i < adjacent_triangle_count(t); ++i) {
        auto ti = adjacent_triangle(t, i);
        if(is_valid(ti)) {
            return proxy(m,
                TriangleEdgeIndex(std::array{index(t), index(ti)}));
        }
    }
    throw std::logic_error("Free standing triangle.");
}

}
