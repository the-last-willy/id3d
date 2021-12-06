#pragma once

#include "terrain/all.hpp"

inline
void update_color_using_wetness(Terrain& t) {
    auto c = value_accessor(t.color);
    auto w = value_accessor(t.wetness);

    auto extent = upper_bound(t.wetness.range);

    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        auto val = w(i, j) / extent;
        c(i, j) = agl::vec3(val);
    }
}
