#pragma once

#include "mesh.hpp"
#include "topology.hpp"

#include "face_vertex/proxy/triangle/proxy.hpp"

namespace face_vertex {

inline
TriangleProxy create_triangle(Mesh& m) {
    topology(m).triangles.emplace_back();
    return proxy(m, TriangleIndex(triangle_count(m) - 1));
}

}
