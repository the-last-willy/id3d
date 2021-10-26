#pragma once

#include "index.hpp"
#include "mesh_topology.hpp"
#include "proxy.hpp"

namespace face_vertex {

inline
VertexTopology& vertex_topology(VertexTopologyProxy vtp) {
    return mesh_topology(vtp).vertices[index(vtp)];
}

}
