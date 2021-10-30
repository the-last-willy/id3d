#pragma once

#include "mesh_topology.hpp"
#include "proxy.hpp"

namespace face_vertex {

inline
TriangleTopologyProxy adjacent_triangle(
    TriangleTopologyProxy ttp, 
    uint32_t i)
{
    return proxy(mesh_topology(ttp), ttp->triangles[i]);
}

}
