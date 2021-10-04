#pragma once

#include "index.hpp"
#include "mesh.hpp"
#include "proxy.hpp"

namespace face_vertex {

inline
VertexTopology& topology(VertexProxy vp) {
    return mesh(vp).topology.vertices[index(vp)];
}

}
