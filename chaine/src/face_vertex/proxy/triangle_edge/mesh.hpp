#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
auto& mesh(TriangleEdgeProxy tep) {
    return tep.mesh;
}

}
