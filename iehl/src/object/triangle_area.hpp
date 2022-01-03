#pragma once

#include "object_group.hpp"

inline
auto triangle_area(const ObjectGroup& og, std::size_t i) {
    auto tis = og.topology.triangle_indices[i];
    auto vp0 = og.vertex_attributes.positions[tis[0]];
    auto vp1 = og.vertex_attributes.positions[tis[1]];
    auto vp2 = og.vertex_attributes.positions[tis[2]];
    return length(cross(vp1 - vp0, vp2 - vp0)) / 2.f;
}
