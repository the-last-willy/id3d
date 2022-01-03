#pragma once

#include "material_properties.hpp"
#include "opengl.hpp"

#include <span>
#include <vector>

struct MaterialGroup {
    std::vector<MaterialProperties> material_properties;
    gl::Buffer material_properties_ssbo;

    gl::Sampler albedo_sampler;
    gl::Texture albedo_texture_array = GL_TEXTURE_2D_ARRAY;
};
