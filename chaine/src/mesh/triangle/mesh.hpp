#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
Mesh& mesh(TriangleProxy tp) {
    return *tp.mesh;
}

}
