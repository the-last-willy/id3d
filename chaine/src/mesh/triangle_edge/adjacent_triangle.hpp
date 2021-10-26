#pragma once

#include "index.hpp"
#include "mesh.hpp"
#include "proxy.hpp"

#include "mesh/triangle/proxy.hpp"

namespace face_vertex {

inline
auto adjacent_triangle(TriangleEdgeProxy tep, uint16_t i) {
    return proxy(mesh(tep), index(tep).value[i]);
}

}
