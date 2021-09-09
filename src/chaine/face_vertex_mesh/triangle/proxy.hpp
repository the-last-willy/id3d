#pragma once

#include "chaine/face_vertex_mesh/mesh/mesh.hpp"
#include "chaine/face_vertex_mesh/vertex/proxy.hpp"
#include "index.hpp"

#include "range/v3/view/iota.hpp"
#include "range/v3/view/transform.hpp"

namespace chaine::face_vertex_mesh {

struct TriangleProxy {
    Mesh& mesh;
    TriangleIndex index;
};

inline
TriangleProxy proxy(Mesh& m, TriangleIndex ti) {
    return TriangleProxy{m, ti};
}

inline
TriangleIndex index(TriangleProxy tp) {
    return tp.index;
}

inline
auto& topology(TriangleProxy tp) {
    return tp.mesh.topology.triangles[index(tp)];
}

inline
uint32_t edge_count(TriangleProxy) {
    return 3;
}

inline
TriangleProxy adjacent_triangle(TriangleProxy tp, uint32_t i) {
    return proxy(tp.mesh, tp.mesh.topology.triangles[index(tp)].triangles[i]);
}

inline
auto vertex(TriangleProxy tp, uint32_t i) {
    return proxy(tp.mesh, tp.mesh.topology.triangles[index(tp)].vertices[i]);
}

inline
uint32_t vertex_count(TriangleProxy) {
    return 3;
}


inline
auto vertices(TriangleProxy tp) {
    return ranges::views::ints(uint32_t(0), vertex_count(tp))
    | ranges::views::transform([tp] (auto i) {
        return proxy(tp.mesh, VertexIndex{i}); });
}

inline
auto barycenter(TriangleProxy tp) {
    auto p0 = position(vertex(tp, 0));
    auto p1 = position(vertex(tp, 1));
    auto p2 = position(vertex(tp, 2));
    return (p0 + p1 + p2) / 3.f;
}

}
