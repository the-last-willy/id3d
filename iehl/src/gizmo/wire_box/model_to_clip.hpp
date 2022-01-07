#pragma once

#include "opengl.hpp"

#include <agl/common/all.hpp>
#include <agl/engine/all.hpp>

inline
agl::Mat4 wire_box_model_to_clip(
    const agl::common::Interval<agl::Vec4>& i)
{
    // auto hl = length(i) / 2;
    // auto mp = midpoint(i);
    // return agl::mat4(
    //     hl[0],   0.f,   0.f, 0.f,
    //       0.f, hl[1],   0.f, 0.f,
    //       0.f,   0.f, hl[2], 0.f,
    //     mp[0], mp[1], mp[2], 1.f);
    return agl::translation(midpoint(i).xyz())
    * agl::scaling3(length(i).xyz() / 2.f);
}
