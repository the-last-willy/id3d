#pragma once

#include <agl/engine/all.hpp>

#include <array>
#include <vector>

struct Scene {
    // Geometry.

    std::vector<unsigned> triangle_material_ids;
    std::vector<agl::Vec3> vertex_normals;
    std::vector<agl::Vec3> vertex_positions;
    std::vector<agl::Vec2> vertex_texcoords;

    // Topology.

    std::vector<std::array<unsigned, 3>> triangle_indices;

    // Cached.

    agl::common::Interval<agl::Vec3> bounding_box;

    // GPU.

    eng::Program program;

    agl::Buffer index_buffer;
    agl::Buffer normal_buffer;
    agl::Buffer position_buffer;
    agl::Buffer texcoord_buffer;

    agl::VertexArray vertex_array;
};
