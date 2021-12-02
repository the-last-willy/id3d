#pragma once

#include "terrain/all.hpp"

inline
void update_colors_for_shading(Terrain& t) {
    auto c = value_accessor(t.color);
    auto g = value_accessor(t.gradient);
    auto l = value_accessor(t.laplacian);
    auto light = agl::vec2(-1.f, 1.f);

    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        c(i, j) = agl::vec3(0.5f + 0.35f * ((agl::dot(g(i, j), light) + 1) * 0.5f));
    }
}
