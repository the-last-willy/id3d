#pragma once

#include "adjacent_triangles.hpp"
#include "proxy.hpp"

#include "face_vertex/proxy/triangle/substitute_vertex.hpp"

#include <vector>

namespace face_vertex {

inline
VertexProxy substitute_by(VertexProxy vp, VertexIndex vi) {
    auto triangles = std::vector<TriangleProxy>();
    // Get all adjacent triangles.
    for(auto&& t : adjacent_triangles(vp)) {
        triangles.push_back(std::move(t));
    }
    // Replace vertex in all adjacent triangles.
    for(auto&& t : triangles) {
        substitute_vertex(t, vp, vi);
    }
    // This could be done in a single pass with O(1) memory.
    // However modifying the topology would invalidates the iterator.
    // Need to do this without the iterator then.
    // (Not very hard to do).
    return vp;
}

}
