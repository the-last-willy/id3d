#pragma once

#include "mesh.hpp"
#include "topology.hpp"

#include "mesh/triangle/proxy.hpp"

namespace face_vertex {

inline
TriangleProxy create_triangle(Mesh& m) {
    return proxy(m, index(create_triangle(topology(m))));
}

}
