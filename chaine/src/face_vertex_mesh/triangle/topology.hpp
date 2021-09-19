#pragma once

#include "face_vertex_mesh/vertex/index.hpp"
#include "index.hpp"

#include <array>

namespace chaine::face_vertex_mesh {

struct TriangleTopology {
    // RENAME INTO 'adjacent_triangles'.
    std::array<TriangleIndex, 3> triangles;
    std::array<VertexIndex, 3> vertices;
};

}
