#pragma once

#include "proxy.hpp"
#include "topology.hpp"

namespace face_vertex {

inline
void invalidate(VertexProxy vp) {
    return invalidate(topology(vp));
}

}
