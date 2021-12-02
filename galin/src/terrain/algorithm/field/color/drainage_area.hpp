#pragma once

#include "terrain/all.hpp"

inline
void update_colors_using_drainage_area(Terrain& t) {
    auto c = value_accessor(t.color);
    auto da = value_accessor(t.drainage_area);

    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        c(i, j) = agl::vec3(1.f / (da(i, j) / 10.f + 1.f));
    }
}
