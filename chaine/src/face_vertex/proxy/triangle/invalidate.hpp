#pragma once

#include "proxy.hpp"
#include "topology.hpp"

namespace face_vertex {

inline
void invalidate(TriangleProxy tp) {
    topology(tp).triangles[0] = tp;
    topology(tp).triangles[1] = tp;
    topology(tp).triangles[2] = tp;
}

}
