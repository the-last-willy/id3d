#pragma once

#include "scene.hpp"

inline
auto centroid(const Scene& s, std::size_t i) {
    auto tis = s.object_group.triangle_indices[i];
    auto vp0 = s.vertex_attribute_group.positions[tis[0]];
    auto vp1 = s.vertex_attribute_group.positions[tis[1]];
    auto vp2 = s.vertex_attribute_group.positions[tis[2]];
    return (vp0 + vp1 + vp2) / 3.f;
}
