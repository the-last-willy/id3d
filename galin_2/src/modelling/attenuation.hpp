#pragma once

#include <agl/engine/all.hpp>

inline
float no_attenuation(agl::Vec3) {
    return 1.f;
}

inline
float linear_attenuation(agl::Vec3 v) {
    auto l = agl::min(length(v), 1.f);
    return 1.f - l;
}

inline
float cubic_attenuation(agl::Vec3 v) {
    auto l = agl::min(length(v), 1.f);
    return l * l * (l * 2.f - 3.f) + 1.f;
}
