#pragma once

#include "terrain/all.hpp"

inline
void update_colors_using_slope(Terrain& t) {
    auto c = value_accessor(t.color);
    auto s = value_accessor(t.slope);

    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        c(i, j) = agl::vec3(1.f / (s(i, j) + 1.f));
    }
}
