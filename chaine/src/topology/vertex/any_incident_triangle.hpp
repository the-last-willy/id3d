#pragma once

#include "index.hpp"
#include "mesh_topology.hpp"
#include "proxy.hpp"

namespace face_vertex {

inline
TriangleTopologyProxy any_incident_triangle(VertexTopologyProxy vtp) {
    return proxy(mesh_topology(vtp), TriangleIndex(vtp->triangle));
}

}
