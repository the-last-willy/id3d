#pragma once

#include "terrain/all.hpp"

inline
void update_colors_using_normals(Terrain& t) {
    auto c = value_accessor(t.color);
    auto n = value_accessor(t.normal);

    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        c(i, j) = 0.5f * n(i, j) + 0.5f;
    }
}
