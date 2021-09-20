#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
auto is_ghost(VertexProxy vp) {
    return topology(vp).is_ghost;
}

}
