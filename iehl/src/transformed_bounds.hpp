#pragma once

#include <agl/common/all.hpp>

inline
auto transformed_bounds(const Interval<agl::Vec4>& i, const agl::Mat4& m) {
    auto lb = lower_bound(i);
    auto ub = upper_bound(i);
    auto corners = std::array{
        agl::vec3(lb[0], lb[1], lb[2], 1.f),
        agl::vec3(lb[0], lb[1], ub[2], 1.f),
        agl::vec3(lb[0], ub[1], lb[2], 1.f),
        agl::vec3(lb[0], ub[1], ub[2], 1.f),
        agl::vec3(ub[0], lb[1], lb[2], 1.f),
        agl::vec3(ub[0], lb[1], ub[2], 1.f),
        agl::vec3(ub[0], ub[1], lb[2], 1.f),
        agl::vec3(ub[0], ub[1], ub[2], 1.f)};
    for(auto& c : corners) {
        auto homogeneous = m * c;
        c = homogeneous / homogeneous[3];
    }
    auto bounds = agl::common::interval(corners[0]);
    for(std::size_t i = 1; i < 8; ++i) {
        extend(frustum_world_bounds, corners[i]);
    }
    return 
}
