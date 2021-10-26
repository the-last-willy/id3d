#pragma once

#include "vertex/proxy.hpp"
#include "topology.hpp"

namespace face_vertex {

inline
VertexTopologyProxy create_vertex(Topology& t) {
    auto vi = VertexIndex(static_cast<uint32_t>(size(t.vertices)));
    t.vertex_count += 1;
    t.vertices.emplace_back();
    return proxy(t, vi);
}

}