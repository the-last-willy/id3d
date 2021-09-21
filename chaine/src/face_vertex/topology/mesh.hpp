#pragma once

#include "triangle.hpp"
#include "vertex.hpp"

#include <vector>

namespace face_vertex {

struct Topology {
    std::vector<TriangleTopology> triangles;
    std::vector<VertexTopology> vertices;
};

}
