#pragma once

#include "terrain/all.hpp"

inline
void update_colors_using_slope(Terrain& t) {
    auto c = value_accessor(t.color);
    auto s = value_accessor(t.slope);

    auto extent = upper_bound(t.slope.range);

    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        auto val = s(i, j) / extent;
        c(i, j) = agl::vec3(val);
    }

    update_road(t);
}
