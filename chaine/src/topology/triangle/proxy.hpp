#pragma once

#include "mesh/topology.hpp"
#include "topology.hpp"

namespace face_vertex {

struct TriangleTopologyProxy {
    Topology* topology;
    TriangleIndex index;

    auto& operator*() const {
        return topology->triangles[index];
    }

    auto* operator->() const {
        return &topology->triangles[index];
    }
};

inline
TriangleTopologyProxy proxy(Topology& t, TriangleIndex ti) {
    return TriangleTopologyProxy(&t, ti);
}

}
