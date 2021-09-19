#pragma once

#include "face_vertex_mesh/triangle/topology.hpp"
#include "face_vertex_mesh/vertex/topology.hpp"

#include <vector>

namespace chaine::face_vertex_mesh {

struct Topology {
    std::vector<TriangleTopology> triangles;
    std::vector<VertexTopology> vertices;
};

}
