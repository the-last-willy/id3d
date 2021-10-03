#pragma once

#include "index.hpp"
#include "proxy.hpp"

namespace face_vertex {

inline
VertexTopology& topology(VertexProxy vp) {
    return vp.mesh.topology.vertices[index(vp)];
}

}
