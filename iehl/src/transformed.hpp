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
