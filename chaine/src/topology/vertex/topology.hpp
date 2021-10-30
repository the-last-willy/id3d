#pragma once

#include "mesh/index/triangle.hpp"

namespace face_vertex {

struct VertexTopology {
    TriangleIndex triangle;

    // TriangleIndex adjacent_triangle;
};

}
