#pragma once

#include "topology.hpp"

namespace face_vertex {

inline
VertexIndex create_vertex(Topology& t) {
    auto i = static_cast<uint32_t>(size(t.vertices));
    t.vertex_count += 1;
    return VertexIndex(i);
}

}