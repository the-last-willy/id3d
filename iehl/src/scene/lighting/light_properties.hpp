#pragma once

#include <agl/glsl/all.hpp>

struct LightProperties {
    // 'vec4' to have the same memory layout as on GPU.

    agl::Vec4 attenuation;
    agl::Vec4 position;
    agl::Vec4 rgb_color;
};

inline
bool are_mergeable(const LightProperties& l0, const LightProperties& l1) {
    return distance(l0.position, l1.position) < 0.1f;
}

inline
LightProperties merged(const LightProperties& l0, const LightProperties& l1) {
    auto m = LightProperties();
    m.attenuation = 1.f / (1.f / l0.attenuation + 1.f / l1.attenuation);
    m.position = (l0.position + l1.position) / 2.f;
    m.rgb_color = (l0.rgb_color + l1.rgb_color) / 2.f;
    return m;
}
