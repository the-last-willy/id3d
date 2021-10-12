#pragma once

#include "proxy.hpp"
#include "topology.hpp"

namespace face_vertex {

inline
void invalidate(TriangleProxy tp) {
    // topology(tp).adjacent_triangles[0] = tp;
    // topology(tp).adjacent_triangles[1] = tp;
    // topology(tp).adjacent_triangles[2] = tp;
    
}

}
