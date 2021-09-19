#pragma once

#include "triangle_index.hpp"
#include "triangle_topology.hpp"

#include <agl/all.hpp>
#include <agl/engine/all.hpp>

#include <memory>
#include <vector>

namespace chaine::triangle_mesh {

struct Topology {
    std::vector<agl::Uvec3> triangle_indices = {};
    std::vector<agl::Vec3> vertex_positions = {};

    std::vector<TriangleTopology> triangles = {};
};

constexpr
auto at(const Topology& t, TriangleIndex i) {
    (void) t;
    (void) i;
}

}
