#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
auto& normal(VertexProxy vp) {
    return vp.mesh.geometry.vertex_normals[vp.index];
}

}
