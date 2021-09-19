#pragma once

#include "mesh.hpp"
#include "vertex_index.hpp"

#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>

namespace chaine::triangle_mesh {

struct VertexProxy {
    const Mesh& mesh;
    VertexIndex index = {};
};

inline
auto proxy(const Mesh& m, VertexIndex vi) {
    return VertexProxy{m, vi};
}

inline
auto& position(VertexProxy vp) {
    return vp.mesh.geometry.vertex_positions[vp.index.value];
}

////////////////////////////////////////////////////////////////////////////////

inline
auto vertices(const Mesh& m) {
    return ranges::views::ints(uint32_t(0), vertex_count(m))
    | ranges::views::transform([&m] (auto i) {
        return proxy(m, VertexIndex{i}); });
}

}
