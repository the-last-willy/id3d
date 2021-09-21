#pragma once

#include "vertex_index.hpp"

namespace triangle_mesh {

struct EdgeIndex {
    VertexIndex head, tail;
};

}
