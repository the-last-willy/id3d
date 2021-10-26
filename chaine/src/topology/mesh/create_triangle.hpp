#pragma once

#include "triangle/proxy.hpp"
#include "topology.hpp"

namespace face_vertex {

inline
TriangleTopologyProxy create_triangle(Topology& t) {
    auto ti = TriangleIndex(static_cast<uint32_t>(size(t.triangles)));
    t.triangle_count += 1;
    auto&& tr = t.triangles.emplace_back();
    for(auto& at : tr.triangles) {
        at = ti;
    }
    return proxy(t, ti);
}

}