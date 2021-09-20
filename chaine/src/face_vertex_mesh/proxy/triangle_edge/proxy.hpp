#pragma once

#include "face_vertex_mesh/index/triangle_edge.hpp"
#include "face_vertex_mesh/mesh/mesh.hpp"

namespace chaine::face_vertex_mesh {

struct TriangleEdgeProxy {
    Mesh& mesh;
    TriangleEdgeIndex index;
};

inline
TriangleEdgeProxy proxy(Mesh& m, TriangleEdgeIndex tei) {
    return TriangleEdgeProxy(m, tei);
}

}
