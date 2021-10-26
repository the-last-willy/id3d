#pragma once

#include "../triangle/topology.hpp"
#include "../vertex/topology.hpp"

#include <vector>

namespace face_vertex {

struct Topology {
    uint32_t triangle_count = 0;
    std::vector<TriangleTopology> triangles;

    uint32_t vertex_count = 0;
    std::vector<VertexTopology> vertices;
};

}
