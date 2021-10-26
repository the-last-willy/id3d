#pragma once

#include "adjacent_triangle.hpp"
#include "proxy.hpp"
#include "adjacent_triangle_count.hpp"

#include "mesh/triangle_edge/proxy.hpp"

#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>

namespace face_vertex {

inline
auto adjacents(TriangleProxy tp) {
    return ranges::views::ints(uint32_t(0), adjacent_triangle_count(tp))
    | ranges::views::transform([tp](uint32_t i) {
        return proxy(mesh(tp), adjacent_triangle(tp, i)); });
}

}
