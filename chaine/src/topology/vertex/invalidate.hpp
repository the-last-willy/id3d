#pragma once

#include "mesh_topology.hpp"
#include "proxy.hpp"
#include "vertex_topology.hpp"

namespace face_vertex {

inline
void invalidate(VertexTopologyProxy vtp) {
    mesh_topology(vtp).vertex_count -= 1;
    vtp->triangle = TriangleIndex(uint32_t(-1));
}

}
