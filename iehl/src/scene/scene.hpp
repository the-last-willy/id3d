#pragma once

#include <agl/engine/all.hpp>

#include <array>
#include <vector>

struct Material {
    std::array<float, 4> color_factor;
};

struct Scene {
    // Geometry.

    std::vector<int> triangle_material_ids;

    std::vector<agl::Vec3> vertex_normals;
    std::vector<agl::Vec3> vertex_positions;
    std::vector<agl::Vec2> vertex_texcoords;

    // Topology.

    std::vector<std::array<unsigned, 3>> triangle_indices;

    // Materials.

    std::vector<Material> materials;

    // Cached.

    agl::common::Interval<agl::Vec3> bounds;

    // GPU.

    eng::Program program;

    agl::Buffer index_buffer;
    agl::Buffer normal_buffer;
    agl::Buffer position_buffer;
    agl::Buffer texcoord_buffer;

    agl::VertexArray vertex_array;

    eng::Texture albedo_array_texture;

    agl::Buffer material_ssbo;
    agl::Buffer triangle_material_id_ssbo;
};
