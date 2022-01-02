#pragma once

#include "light_properties.hpp"
#include "opengl.hpp"

#include <vector>

struct LightGroup {
    std::vector<LightProperties> light_properties;
    agl::opengl::Buffer light_properties_ssbo;
};
