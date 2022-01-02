#pragma once

#include "opengl.hpp"

#include <agl/glsl/all.hpp>

#include <array>
#include <vector>

struct VertexAttributeGroup {
    // Should only keep the buffers in there ?

    std::vector<agl::Vec3> normals;
    gl::Buffer normal_buffer;

    std::vector<agl::Vec3> positions;
    gl::Buffer position_buffer;

    std::vector<agl::Vec2> texcoords;
    gl::Buffer texcoords_buffer;
};
