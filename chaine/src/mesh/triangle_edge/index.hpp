#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
TriangleEdgeIndex index(TriangleEdgeProxy tep) {
    return tep.index;
}

}
