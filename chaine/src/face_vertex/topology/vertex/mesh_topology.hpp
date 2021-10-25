#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
Topology& mesh_topology(VertexTopologyProxy vtp) {
    return *vtp.topology;
}

}
