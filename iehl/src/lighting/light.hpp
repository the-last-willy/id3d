#pragma once

#include <agl/glsl/all.hpp>

struct Light {
    // 'vec4' to have the same memory layout as on GPU.

    agl::Vec4 attenuation;
    // RGB.
    agl::Vec4 color;
    agl::Vec4 position;
};
