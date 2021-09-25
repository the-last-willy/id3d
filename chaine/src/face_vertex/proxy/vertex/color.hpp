#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
auto& color(VertexProxy vp) {
    return vp.mesh.geometry.vertex_colors[vp.index];
}


}
