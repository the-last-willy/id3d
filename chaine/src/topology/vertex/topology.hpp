#pragma once

#include "mesh/index/triangle.hpp"

namespace face_vertex {

struct VertexTopology {
    TriangleIndex triangle;

    // Both of these are the naive way of doing this.
    // It's costs too much memory for no valid reason.
    // Find a way that is O(1).
    // Maybe reserving index 0 or max_val for invalid values.
    // bool is_ghost = false;
    // bool is_valid = true;

    // TriangleIndex adjacent_triangle;
};

}
