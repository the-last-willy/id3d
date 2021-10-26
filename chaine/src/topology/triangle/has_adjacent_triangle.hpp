#pragma once

#include "mesh_topology.hpp"
#include "proxy.hpp"
#include "triangle_topology.hpp"

namespace face_vertex {

inline
bool has_adjacent_triangle(TriangleTopologyProxy ttp, uint32_t i) {
    return ttp->triangles[i] != index(ttp);
}

}
