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

}
