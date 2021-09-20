#pragma once

#include "mesh.hpp"
#include "proxy.hpp"
#include "topology.hpp"
#include "vertex_count.hpp"

#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>

namespace face_vertex {

inline
auto vertices(TriangleProxy tp) {
    return ranges::views::ints(uint32_t(0), vertex_count(tp))
    | ranges::views::transform([tp] (auto i) {
        return proxy(mesh(tp), topology(tp).vertices[i]); });
}

}
