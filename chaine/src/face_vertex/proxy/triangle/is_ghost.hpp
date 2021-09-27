#pragma once

#include "proxy.hpp"
#include "topology.hpp"

namespace face_vertex {

inline
auto is_ghost(TriangleProxy tp) {
    return topology(tp).is_ghost;
}

}
