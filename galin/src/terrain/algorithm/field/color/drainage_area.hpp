#pragma once

#include "terrain/all.hpp"

inline
void update_colors_using_drainage_area(Terrain& t) {
    auto c = value_accessor(t.color);
    auto da = value_accessor(t.drainage_area);

    auto compression = [](float f) {
        return std::log(f);
    };

    auto max = upper_bound(t.drainage_area.range);
    auto extent = compression(max);

    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        c(i, j) = agl::vec3(0.f, 0.f, compression(da(i, j)) / extent);
    }
}
