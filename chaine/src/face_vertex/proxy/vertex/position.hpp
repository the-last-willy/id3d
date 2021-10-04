#pragma once

#include "mesh.hpp"
#include "proxy.hpp"

namespace face_vertex {

inline
auto& position(VertexProxy vp) {
    return mesh(vp).geometry.vertex_positions[vp.index];
}

}
