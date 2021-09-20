#pragma once

#include "proxy.hpp"

namespace chaine::face_vertex_mesh {

inline
auto& mesh(TriangleEdgeProxy tep) {
    return tep.mesh;
}

}
