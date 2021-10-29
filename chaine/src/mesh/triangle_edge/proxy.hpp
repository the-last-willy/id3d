#pragma once

#include "mesh/index/triangle_edge.hpp"
#include "mesh/mesh/mesh.hpp"

namespace face_vertex {

struct TriangleEdgeProxy {
    Mesh& mesh;
    TriangleEdgeIndex index;
};

inline
auto proxy(Mesh& m, TriangleEdgeIndex tei) {
    return TriangleEdgeProxy(m, tei);
}

inline
auto proxy(Mesh& m, TriangleIndex t0, TriangleIndex t1) {
    return proxy(m, TriangleEdgeIndex(std::array{t0, t1}));
}

}
