#pragma once

#include "mesh.hpp"
#include "proxy.hpp"

namespace face_vertex {

inline
auto topology(TriangleProxy tp) {
    return proxy(topology(mesh(tp)), index(tp));
}

}
