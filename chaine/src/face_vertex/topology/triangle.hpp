#pragma once

#include "face_vertex/index/triangle.hpp"
#include "face_vertex/index/vertex.hpp"

#include <array>

namespace face_vertex {

struct TriangleTopology {
    // RENAME INTO 'adjacents' or 'adjacent_triangles' ?
    std::array<TriangleIndex, 3> triangles;
    std::array<VertexIndex, 3> vertices;

    // DO SOMETHING BETTER, TOO EXPENSIVE.
    bool is_ghost = false;
};

}
