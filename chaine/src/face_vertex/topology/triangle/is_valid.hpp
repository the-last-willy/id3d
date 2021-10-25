#pragma once

#include "proxy.hpp"
#include "triangle_topology.hpp"

namespace face_vertex {

inline
bool is_valid(TriangleTopologyProxy ttp) {
    return triangle_topology(ttp).vertices[0] != VertexIndex(uint32_t(-1));
}

}
