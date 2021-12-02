#pragma once

#include "terrain/all.hpp"

inline
void update_colors_for_shading(Terrain& t) {
    auto c = value_accessor(t.color);

    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        c(i, j) = agl::vec3(0.5f);
    }
}
