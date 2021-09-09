#pragma once

#include "chaine/face_vertex_mesh/mesh/mesh.hpp"
#include "chaine/face_vertex_mesh/vertex/proxy.hpp"
#include "index.hpp"

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
TriangleProxy adjacent_triangle(TriangleProxy tp, uint32_t i) {
    return proxy(tp.mesh, tp.mesh.topology.triangles[tp.index].triangles[i]);
}

inline
auto vertex(TriangleProxy tp, uint32_t i) {
    return proxy(tp.mesh, tp.mesh.topology.triangles[tp.index].vertices[i]);
}

inline
auto vertices(TriangleProxy) {
    throw std::runtime_error("Not implemented.");
}

inline
auto barycenter(TriangleProxy tp) {
    auto p0 = position(vertex(tp, 0));
    auto p1 = position(vertex(tp, 1));
    auto p2 = position(vertex(tp, 2));
    return (p0 + p1 + p2) / 3.f;
}

}
