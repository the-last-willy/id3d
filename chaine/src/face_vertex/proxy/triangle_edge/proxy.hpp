#pragma once

#include "face_vertex/index/triangle_edge.hpp"
#include "face_vertex/mesh.hpp"

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
