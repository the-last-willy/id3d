#pragma once

#include "mesh_topology.hpp"
#include "proxy.hpp"
#include "triangle_topology.hpp"

namespace face_vertex {

inline
bool has_incident_vertex(TriangleTopologyProxy ttp, VertexIndex vi) {
    for(uint32_t i = 0; i < 3; ++i) {
        if(ttp->vertices[i] == vi) {
            return true;
        }
    }
    return false;
}

}
