#pragma once

#include "scene.hpp"

#include <agl/engine/all.hpp>

inline
auto bounding_box(const Scene& s) {
    auto bb = agl::common::interval(s.vertex_attribute_group.positions[0]);
    for(auto& vp : s.vertex_attribute_group.positions) {
        extend(bb, vp);
    }
    return bb;
}
