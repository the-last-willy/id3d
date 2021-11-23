#pragma once

#include <agl/engine/all.hpp>

#include <array>
#include <vector>

struct Scene {
    // Geometry.

    std::vector<unsigned> triangle_material_ids;
    std::vector<std::array<float, 3>> vertex_normals;
    std::vector<std::array<float, 3>> vertex_positions;
    std::vector<std::array<float, 2>> vertex_texcoords;

    // Topology.

    std::vector<std::array<unsigned, 3>> triangle_indices;

    // GPU.

    eng::Program program;

    agl::Buffer index_buffer;
    agl::Buffer normal_buffer;
    agl::Buffer position_buffer;
    agl::Buffer texcoord_buffer;

    agl::VertexArray vertex_array;
};
