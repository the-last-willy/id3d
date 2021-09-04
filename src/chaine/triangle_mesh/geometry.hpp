#pragma once

#include <vector>

#include <agl/all.hpp>

namespace chaine::triangle_mesh {

struct Geometry {
    std::vector<agl::Vec3> vertex_positions = {};
};

}
