#pragma once

#include "proxy.hpp"
#include "topology.hpp"

namespace face_vertex {

inline
void invalidate(TriangleProxy tp) {
    invalidate(topology(tp));
}

}
