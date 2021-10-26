#pragma once

#include "mesh_topology.hpp"
#include "proxy.hpp"
#include "triangle_topology.hpp"

namespace face_vertex {

inline
void invalidate(TriangleTopologyProxy ttp) {
    mesh_topology(ttp).triangle_count -= 1;
    ttp->vertices[0] = VertexIndex(uint32_t(-1));
}

}
