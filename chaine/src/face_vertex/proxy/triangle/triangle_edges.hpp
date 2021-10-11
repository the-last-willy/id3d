#pragma once

#include "proxy.hpp"
#include "adjacent_triangle_count.hpp"

#include "face_vertex/proxy/triangle_edge/proxy.hpp"

#include <range/v3/view/filter.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>

namespace face_vertex {

inline
auto triangle_edges(TriangleProxy) {
    // return ranges::views::ints(uint32_t(0), adjacent_triangle_count(tp))
    // | ranges::views::transform()
}

}
