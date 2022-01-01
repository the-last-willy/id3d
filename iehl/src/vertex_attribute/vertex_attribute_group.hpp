#pragma once

#include "opengl.hpp"

#include <agl/glsl/all.hpp>

#include <array>
#include <vector>

struct VertexAttributeGroup {
    // Topology.

    // Should it be in geometry ?
    // No GPU buffer because triangle indices might be ordered differently ?
    std::vector<std::array<GLuint, 3>> triangle_indices;

    // Triangle geometry.

    std::vector<int> triangle_material_ids;
    gl::Buffer triangle_material_id_ssbo;

    // Unused ?
    std::vector<std::size_t> triangle_object_ids; 

    // Vertex geometry.

    std::vector<agl::Vec3> normals;
    gl::Buffer normal_buffer;

    std::vector<agl::Vec3> positions;
    gl::Buffer position_buffer;

    std::vector<agl::Vec2> texcoords;
    gl::Buffer texcoords_buffer;
};
