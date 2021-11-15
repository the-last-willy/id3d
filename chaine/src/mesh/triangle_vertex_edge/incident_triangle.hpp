#pragma once

#include "index.hpp"
#include "mesh.hpp"
#include "proxy.hpp"

namespace face_vertex {

inline
auto incident_triangle(TriangleVertexEdgeProxy tvep) {
    return proxy(mesh(tvep), std::get<TriangleIndex>(index(tvep).value));
}

}
