#pragma once

#include "proxy.hpp"
#include "topology.hpp"

namespace face_vertex {

inline
auto is_ghost(VertexProxy vp) {
    auto t = topology(vp);
    return topology(vp).is_ghost;
}

}
