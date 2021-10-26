#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
auto& normal(VertexProxy vp) {
    return mesh(vp).geometry.vertex_normals[vp.index];
}

}
