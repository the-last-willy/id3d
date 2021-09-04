#pragma once

#include "mesh.hpp"
#include "triangle_index.hpp"
#include "vertex_proxy.hpp"

#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>

namespace chaine::triangle_mesh {

struct TriangleProxy {
    const Mesh& mesh;
    TriangleIndex index;
};

inline
auto proxy(const Mesh& m, TriangleIndex ti) {
    return TriangleProxy(m, ti);
}

inline
auto& topology(TriangleProxy tp) {
    return tp.mesh.topology.triangles[tp.index];
}

inline
auto vertex(TriangleProxy tp, uint32_t i) {
    return proxy(tp.mesh, VertexIndex{topology(tp).vertices[i]});
}

inline
auto vertices(TriangleProxy tp) {
    return ranges::views::ints(uint32_t(0), uint32_t(3))
    | ranges::views::transform([&tp] (uint32_t i) {
        auto& topology = tp.mesh.topology.triangles[tp.index];
        return proxy(tp.mesh, VertexIndex{topology.vertices[i]}); });
}

////////////////////////////////////////////////////////////////////////////////

inline
auto triangles(const Mesh& m) {
    return ranges::views::ints(uint32_t(0), triangle_count(m))
    | ranges::views::transform([&m] (auto i) {
        return proxy(m, TriangleIndex{i}); });
}

}
