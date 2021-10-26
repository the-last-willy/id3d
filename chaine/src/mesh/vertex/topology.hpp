#pragma once

#include "index.hpp"
#include "mesh.hpp"
#include "proxy.hpp"

namespace face_vertex {

inline
auto topology(VertexProxy vp) {
    return proxy(topology(mesh(vp)), index(vp));
}

}
