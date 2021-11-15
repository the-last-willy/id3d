#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
auto& mesh(TriangleVertexEdgeProxy tvep) {
    return tvep.mesh;
}

}
