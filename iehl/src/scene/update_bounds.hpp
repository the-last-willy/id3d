#pragma once

#include "scene.hpp"

inline
void update_bounds(Scene& s) {
    s.bounds = agl::common::interval(s.vertex_positions[0]);
    for(const auto& vp : s.vertex_positions) {
        extend(s.bounds, vp);
    }
}
