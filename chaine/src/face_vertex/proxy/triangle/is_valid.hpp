#pragma once

#include "is_ghost.hpp"
#include "proxy.hpp"
#include "topology.hpp"

namespace face_vertex {

inline
bool is_valid(TriangleProxy tp) {
    return topology(tp).triangles[0] != index(tp)
    && not is_ghost(tp);
}

}
