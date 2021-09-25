#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
auto& position(VertexProxy vp) {
    return vp.mesh.geometry.vertex_positions[vp.index];
}

}
