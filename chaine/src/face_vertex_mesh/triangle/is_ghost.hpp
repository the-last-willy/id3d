#pragma once

#include "proxy.hpp"

namespace chaine::face_vertex_mesh {

inline
auto is_ghost(TriangleProxy tp) {
    return topology(tp).is_ghost;
}

}
