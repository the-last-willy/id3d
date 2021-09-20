#pragma once

#include "mesh.hpp"
#include "proxy.hpp"

namespace face_vertex {

inline
auto& topology(TriangleProxy tp) {
    return mesh(tp).topology.triangles[index(tp)];
}

}
