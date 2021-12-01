#pragma once

#include "terrain/all.hpp"

inline
void update_slope_color_field(Terrain& t) {
    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        auto s = at(t.slopes, i, j);
        at(t.colors, i, j) = agl::vec3(1.f / (s + 1.f));
    }
}
