#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
auto is_ghost(TriangleProxy tp) {
    return topology(tp).is_ghost;
}

}
