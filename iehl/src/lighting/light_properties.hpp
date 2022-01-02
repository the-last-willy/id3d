#pragma once

#include <agl/glsl/all.hpp>

struct LightProperties {
    // 'vec4' to have the same memory layout as on GPU.

    agl::Vec4 attenuation;
    agl::Vec4 position;
    agl::Vec4 rgb_color;
};
