#pragma once

#include "face_vertex/proxy/triangle/proxy.hpp"
#include "mesh.hpp"

#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>

#include <stdexcept>

namespace face_vertex {

inline
auto triangles(Mesh& m) {
    return ranges::views::ints(uint32_t(0), triangle_count(m))
    | ranges::views::transform([&m] (auto i) {
        return proxy(m, TriangleIndex{i}); });
}

}
