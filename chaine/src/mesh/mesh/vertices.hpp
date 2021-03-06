#pragma once

#include "mesh.hpp"

#include "mesh/vertex/is_valid.hpp"
#include "mesh/vertex/proxy.hpp"

#include <range/v3/view/filter.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>

namespace face_vertex {

inline
auto vertices(Mesh& m) {
    return ranges::views::ints(uint32_t(0), vertex_count(m))
    | ranges::views::transform([&m] (auto i) {
        return proxy(m, VertexIndex{i}); })
    | ranges::views::filter([](const VertexProxy& vp) {
        return is_valid(vp); });
}

}
