#pragma once

#include "opengl.hpp"

struct ObjectGroupData {
    // Per object.

    std::vector<agl::common::Interval<agl::Vec4>> object_bounds;
    gl::Buffer object_bounds_ssbo;

    // Per triangle.

    std::vector<int> triangle_material_ids;
    gl::Buffer triangle_material_id_ssbo;

    // Unused ?
    std::vector<std::size_t> triangle_object_ids; 

    // Whole.

    agl::common::Interval<agl::Vec4> bounds;
};
