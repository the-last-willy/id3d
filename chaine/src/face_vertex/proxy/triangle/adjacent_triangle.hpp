#pragma once

#include "index.hpp"
#include "mesh.hpp"
#include "proxy.hpp"
#include "topology.hpp"

namespace face_vertex {

inline
TriangleProxy adjacent_triangle(TriangleProxy tp, uint32_t i) {
    return proxy(
        mesh(tp),
        topology(tp).triangles[i]);
}

}
