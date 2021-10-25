#pragma once

#include "mesh/topology.hpp"
#include "topology.hpp"

namespace face_vertex {

struct TriangleTopologyProxy {
    Topology* topology;
    TriangleIndex index;
};

}
