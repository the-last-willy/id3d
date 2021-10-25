#pragma once

#include "index.hpp"
#include "mesh_topology.hpp"
#include "proxy.hpp"

namespace face_vertex {

inline
TriangleTopology& triangle_topology(TriangleTopologyProxy ttp) {
    return mesh_topology(ttp).triangles[index(ttp)];
}

}
