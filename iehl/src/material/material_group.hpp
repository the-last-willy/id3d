#pragma once

#include "material_properties.hpp"
#include "opengl.hpp"

#include <span>
#include <vector>

struct MaterialGroup {
    std::vector<MaterialProperties> properties;
    gl::Buffer properties_ssbo;

    gl::Sampler albedo_sampler;
    gl::Texture albedo_texture_array = GL_TEXTURE_2D_ARRAY;
};

inline
void update_ssbos(MaterialGroup& mg) {
    gl::NamedBufferStorage(mg.properties_ssbo, std::span(mg.properties));
}
