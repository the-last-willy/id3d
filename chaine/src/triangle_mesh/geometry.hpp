#pragma once

#include <vector>

#include <agl/all.hpp>

namespace triangle_mesh {

struct Geometry {
    std::vector<agl::Uvec3> triangle_indices = {};
    std::vector<agl::Vec3> vertex_positions = {};
};

}
