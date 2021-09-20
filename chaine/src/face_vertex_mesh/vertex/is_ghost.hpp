#pragma once

#include "proxy.hpp"

namespace chaine::face_vertex_mesh {

inline
auto is_ghost(VertexProxy vp) {
    return topology(vp).is_ghost;
}

}
