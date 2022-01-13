#pragma once

#include <agl/common/all.hpp>
#include <agl/glsl/all.hpp>

inline
agl::common::Interval<agl::Vec4> transformed(
    const agl::Mat4& m,
    const agl::common::Interval<agl::Vec4>& i)
{
    auto corners = std::array{
        agl::vec4(lower_bound(i)[0], lower_bound(i)[1], lower_bound(i)[2], 1.f),
        agl::vec4(lower_bound(i)[0], lower_bound(i)[1], upper_bound(i)[2], 1.f),
        agl::vec4(lower_bound(i)[0], upper_bound(i)[1], lower_bound(i)[2], 1.f),
        agl::vec4(lower_bound(i)[0], upper_bound(i)[1], upper_bound(i)[2], 1.f),
        agl::vec4(upper_bound(i)[0], lower_bound(i)[1], lower_bound(i)[2], 1.f),
        agl::vec4(upper_bound(i)[0], lower_bound(i)[1], upper_bound(i)[2], 1.f),
        agl::vec4(upper_bound(i)[0], upper_bound(i)[1], lower_bound(i)[2], 1.f),
        agl::vec4(upper_bound(i)[0], upper_bound(i)[1], upper_bound(i)[2], 1.f)};
    auto tr = agl::common::Interval<agl::Vec4>();
    { // First corner.
        auto tcorner = m * corners[0];
        tcorner /= tcorner[3];
        tr = agl::common::interval(tcorner);
    }
    // Remaining corners.
    for(std::size_t j = 1; j < 8; ++j) {
        auto tcorner = m * corners[j];
        tcorner /= tcorner[3];
        extend(tr, tcorner);
    }
    return tr;
}

inline
bool frustum_culling_are_clip_separated(
    const agl::Mat4& object_to_clip,
    const agl::common::Interval<agl::Vec4>& i)
{
    auto object_corners = std::array{
        agl::vec4(lower_bound(i)[0], lower_bound(i)[1], lower_bound(i)[2], 1.f),
        agl::vec4(lower_bound(i)[0], lower_bound(i)[1], upper_bound(i)[2], 1.f),
        agl::vec4(lower_bound(i)[0], upper_bound(i)[1], lower_bound(i)[2], 1.f),
        agl::vec4(lower_bound(i)[0], upper_bound(i)[1], upper_bound(i)[2], 1.f),
        agl::vec4(upper_bound(i)[0], lower_bound(i)[1], lower_bound(i)[2], 1.f),
        agl::vec4(upper_bound(i)[0], lower_bound(i)[1], upper_bound(i)[2], 1.f),
        agl::vec4(upper_bound(i)[0], upper_bound(i)[1], lower_bound(i)[2], 1.f),
        agl::vec4(upper_bound(i)[0], upper_bound(i)[1], upper_bound(i)[2], 1.f)};
    auto axes = std::array{true, true, true, true, true, true};
    for(std::size_t j = 0; j < 8; ++j) {
        auto clip_corner = object_to_clip * object_corners[j];
        if(clip_corner[0] > -clip_corner[3]) {
            axes[0] = false;
        }
        if(clip_corner[0] < clip_corner[3]) {
            axes[1] = false;
        }
        if(clip_corner[1] > -clip_corner[3]) {
            axes[2] = false;
        }
        if(clip_corner[1] < clip_corner[3]) {
            axes[3] = false;
        }
        if(clip_corner[2] > -clip_corner[3]) {
            axes[4] = false;
        }
        if(clip_corner[2] < clip_corner[3]) {
            axes[5] = false;
        }
    }
    return axes[0] or axes[1] or axes[2] or axes[3] or axes[4] or axes[5];
}
