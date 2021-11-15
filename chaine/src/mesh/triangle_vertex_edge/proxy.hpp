#pragma once

#include "mesh/index/triangle_edge.hpp"
#include "mesh/mesh/mesh.hpp"

namespace face_vertex {

struct TriangleVertexEdgeProxy {
    Mesh& mesh;
    TriangleVertexEdgeIndex index;
};

inline
auto proxy(Mesh& m, TriangleVertexEdgeIndex tvei) {
    return TriangleVertexEdgeProxy(m, tvei);
}

inline
auto proxy(Mesh& m, TriangleIndex ti, VertexIndex vi) {
    return proxy(m, TriangleVertexEdgeIndex(std::make_tuple(ti, vi)));
}

}
