#pragma once

#include "chaine/face_vertex_mesh/vertex/index.hpp"
#include "index.hpp"

#include <array>

namespace chaine::face_vertex_mesh {

struct TriangleTopology {
    std::array<TriangleIndex, 3> triangles;
    std::array<VertexIndex, 3> vertices;
};

}
