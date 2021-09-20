#pragma once

#include "index.hpp"
#include "mesh.hpp"
#include "proxy.hpp"

#include "face_vertex_mesh/triangle/proxy.hpp"

namespace chaine::face_vertex_mesh {

inline
auto adjacent_triangle(TriangleEdgeProxy tep, uint16_t i) {
    return proxy(mesh(tep), index(tep).value[i]);
}

}
