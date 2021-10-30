#pragma once

#include "mesh_topology.hpp"
#include "proxy.hpp"
#include "triangle_topology.hpp"

namespace face_vertex {

inline
bool is_adjacent_to(TriangleTopologyProxy ttp, TriangleIndex ti) {
    for(uint32_t i = 0; i < 3; ++i) {
        if(ttp->triangles[i] == ti) {
            return true;
        }
    }
    return false;
}

}
