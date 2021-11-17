#pragma once

#include "proxy.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>

namespace face_vertex {

inline
float smallest_angle(TriangleProxy tp) {
    auto v0 = vertex(tp, 0);
    auto v1 = vertex(tp, 1);
    auto v2 = vertex(tp, 2);
    auto d0 = position(v1) - position(v0);
    auto d1 = position(v2) - position(v1);
    auto d2 = position(v0) - position(v2);
    auto a0 = std::atan2(length(cross(d0, -d2)), dot(d0, -d2));
    auto a1 = std::atan2(length(cross(d1, -d0)), dot(d1, -d0));
    auto a2 = std::atan2(length(cross(d2, -d1)), dot(d2, -d1));
    auto angles = std::array{a0, a1, a2};
    return *std::ranges::min_element(angles);
}

} 
