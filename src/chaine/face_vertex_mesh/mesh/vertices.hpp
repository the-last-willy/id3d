#pragma once

#include "chaine/face_vertex_mesh/vertex/proxy.hpp"
#include "mesh.hpp"

#include "range/v3/view/iota.hpp"
#include "range/v3/view/transform.hpp"

#include <stdexcept>

namespace chaine::face_vertex_mesh {

inline
auto vertices(Mesh& m) {
    return ranges::views::ints(uint32_t(0), vertex_count(m))
    | ranges::views::transform([&m] (auto i) {
        return proxy(m, VertexIndex{i}); });
}

}
