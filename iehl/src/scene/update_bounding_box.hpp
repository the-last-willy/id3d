#pragma once

#include "scene.hpp"

inline
void update_bounding_box(Scene& s) {
    s.bounding_box = agl::common::interval(s.vertex_positions);
    for(const auto& vp : s.vertex_positions) {
        extend(s.bounding_box, vp);
    }
}
