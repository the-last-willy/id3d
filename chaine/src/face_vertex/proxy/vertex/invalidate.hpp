#pragma once

#include "proxy.hpp"
#include "topology.hpp"

namespace face_vertex {

inline
void invalidate(VertexProxy vp) {
    topology(vp).is_valid = false;
}

}
