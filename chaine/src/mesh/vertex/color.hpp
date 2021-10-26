#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
auto& color(VertexProxy vp) {
    return mesh(vp).geometry.vertex_colors[vp.index];
}


}
