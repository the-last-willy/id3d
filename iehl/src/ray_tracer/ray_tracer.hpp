#pragma once

#include "point_cloud/point_cloud.hpp"

#include <agl/standard/all.hpp>

struct RayTracer {
    std::default_random_engine random
    = std::default_random_engine(agl::standard::random_seed());

    //

    PointCloud pc;

    // Programs.

    gl::Program draw_program;

    // Attribute locations.

    gl::OptAttribLoc draw_color;
    gl::OptAttribLoc draw_normal;
    gl::OptAttribLoc draw_position;

    // Uniform locations.

    gl::OptUniformLoc draw_object_to_clip_position;
    gl::OptUniformLoc draw_object_to_world_normal;
    gl::OptUniformLoc draw_object_to_world_position;
    gl::OptUniformLoc draw_view_world_position;
};
