#pragma once

#include "index.hpp"

#include <stdexcept>

namespace chaine::face_vertex_mesh {

struct TriangleEdgeProxy {
    Mesh& mesh;
    TriangleEdgeIndex index;
};

inline
VertexProxy proxy(Mesh& m, VertexIndex vi) {
    return VertexProxy{m, vi};
}

inline
auto index(TriangleEdgeProxy tep) {
    return tep.index;
}

inline
auto& mesh(TriangleEdgeProxy tep) {
    return tep.mesh;
}

}
