#pragma once

#include "opengl.hpp"

#include <agl/common/all.hpp>
#include <agl/engine/all.hpp>

inline
agl::Mat4 solid_box_model_to_clip(
    const agl::common::Interval<agl::Vec4>& i)
{
    return agl::translation(midpoint(i).xyz())
    * agl::scaling3(length(i).xyz() / 2.f);
}

inline
agl::Mat4 solid_box_model_to_world(
    const eng::Camera& c)
{
    return agl::engine::clip_to_world(c);
}

inline
agl::Mat4 solid_box_object_to_world(
    const agl::common::Interval<agl::Vec4>& i)
{
    return agl::translation(midpoint(i).xyz())
    * agl::scaling3(length(i).xyz() / 2.f);
}
