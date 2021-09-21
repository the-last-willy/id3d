#pragma once

#include "face_vertex/index/triangle.hpp"

namespace face_vertex {

struct VertexTopology {
    TriangleIndex triangle;

    bool is_ghost = false;
};

}
