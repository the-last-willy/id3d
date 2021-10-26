#pragma once

#include "mesh/topology.hpp"
#include "topology.hpp"

namespace face_vertex {

struct VertexTopologyProxy {
    Topology* topology;
    VertexIndex index;

    auto& operator*() const {
        return topology->vertices[index];
    }

    auto* operator->() const {
        return &topology->vertices[index];
    }
};

inline
VertexTopologyProxy proxy(Topology& t, VertexIndex vi) {
    return VertexTopologyProxy(&t, vi);
}

}
