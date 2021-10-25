#pragma once

#include "topology.hpp"

namespace face_vertex {

inline
VertexIndex create_vertex(Topology& t) {
    auto i = static_cast<uint32_t>(size(t.vertices));
    return VertexIndex(i);
}

}