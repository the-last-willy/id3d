#pragma once

#include "proxy.hpp"
#include "topology.hpp"

namespace face_vertex {

inline
bool is_valid(VertexProxy vp) {
    return is_valid(topology(vp));
}

}
