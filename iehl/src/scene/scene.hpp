#pragma once

#include "light/all.hpp"
#include "light_grid/all.hpp"

#include <agl/engine/all.hpp>

#include <array>
#include <vector>

struct Material {
    std::array<float, 4> color_factor;
    std::array<float, 4> emission_factor;
};

inline
bool is_emissive(const Material& m) {
    return m.emission_factor[0] > 0.f
    or m.emission_factor[1] > 0.f
    or m.emission_factor[2] > 0.f;
}

struct Scene {
    // Geometry.

    std::vector<int> triangle_material_ids;
    std::vector<std::size_t> triangle_object_ids; 

    std::vector<agl::Vec3> vertex_normals;
    std::vector<agl::Vec3> vertex_positions;
    std::vector<agl::Vec2> vertex_texcoords;

    // Topology.

    std::vector<std::array<unsigned, 3>> triangle_indices;

    // Materials.

    std::vector<Material> materials;

    // Lights.

    std::vector<Light> lights;

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

    agl::Buffer light_ssbo;
    agl::Buffer material_ssbo;
    agl::Buffer triangle_material_id_ssbo;
};
