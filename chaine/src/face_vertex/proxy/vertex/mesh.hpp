#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
Mesh& mesh(VertexProxy vp) {
    return *vp.mesh;
}

}
