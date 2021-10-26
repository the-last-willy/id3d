#pragma once

#include "mesh.hpp"

#include "mesh/triangle/proxy.hpp"

namespace face_vertex {

inline
TriangleProxy triangle(Mesh& m, uint32_t ti) {
    return proxy(m, TriangleIndex(ti));
}

}
