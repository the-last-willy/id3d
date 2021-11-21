#pragma once

#include "position.hpp"

namespace face_vertex {

inline
auto distance(VertexProxy v0, VertexProxy v1) {
    return length(position(v0) - position(v1));
}

}
