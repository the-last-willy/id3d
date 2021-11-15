#pragma once

#include "index.hpp"
#include "mesh.hpp"
#include "proxy.hpp"

namespace face_vertex {

inline
auto opposite_vertex(TriangleVertexEdgeProxy tvep) {
    return proxy(mesh(tvep), std::get<VertexIndex>(index(tvep).value));
}

}
