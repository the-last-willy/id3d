#pragma once

#include "opengl/draw_elements_parameters.hpp"

#include <agl/engine/all.hpp>

struct FrustumCullingBatch {
    // CPU.

    agl::Mat4 clip_to_world;
    agl::Mat4 world_to_clip;

    agl::common::Interval<agl::Vec3> frustum_clip_bounds;
    agl::common::Interval<agl::Vec3> frustum_world_bounds;

    std::vector<agl::common::Interval<agl::Vec3>> objects_bounds;

    std::vector<DrawElementsParameters> input_draw_parameters;

    std::vector<DrawElementsParameters> output_draw_parameters;

    // GPU.

    agl::Buffer object_bound_ssbo;

    agl::Buffer input_draw_parameter_ssbo;

    agl::Buffer output_count_ssbo;
    agl::Buffer output_draw_parameter_ssbo;
};
