#pragma once

#include "mesh/topology.hpp"
#include "topology.hpp"

namespace face_vertex {

struct VertexTopologyProxy {
    Topology* topology;
    VertexIndex index;
};

}
