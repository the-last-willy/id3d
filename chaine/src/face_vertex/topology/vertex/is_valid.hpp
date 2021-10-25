#pragma once

#include "proxy.hpp"
#include "vertex_topology.hpp"

namespace face_vertex {

inline
bool is_valid(VertexTopologyProxy vtp) {
    return vertex_topology(vtp).triangle != TriangleIndex(uint32_t(-1));
}

}
