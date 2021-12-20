#pragma once

#include <agl/glsl/all.hpp>

struct Light {
    agl::Vec3 attenuation;
    agl::Vec4 color;
    agl::Vec4 position;
};
