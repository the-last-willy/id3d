#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
Topology& mesh_topology(TriangleTopologyProxy ttp) {
    return *ttp.topology;
}

}
