#pragma once

#include "triangle.hpp"
#include "vertex.hpp"

#include <vector>

namespace chaine::face_vertex_mesh {

struct Topology {
    std::vector<TriangleTopology> triangles;
    std::vector<VertexTopology> vertices;
};

}
