#pragma once

#include "mesh.hpp"

#include "face_vertex/proxy/triangle/is_ghost.hpp"
#include "face_vertex/proxy/triangle/proxy.hpp"

#include <range/v3/view/filter.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>

namespace face_vertex {

inline
auto triangles(Mesh& m) {
    return ranges::views::ints(uint32_t(0), triangle_count(m))
    | ranges::views::transform([&m] (auto i) {
        return proxy(m, TriangleIndex(i)); })
    | ranges::views::filter([](const TriangleProxy& tp) {
        return not is_ghost(tp); });
}

}
