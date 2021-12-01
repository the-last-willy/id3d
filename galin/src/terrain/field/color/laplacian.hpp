#pragma once

#include "terrain/all.hpp"

inline
void update_laplacian_color_field(Terrain& t) {
    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        auto l = at(t.laplacian, i, j);
        at(t.colors, i, j) = agl::vec3(
            std::max(  l, 0.f),
            0.f,
            std::max(- l, 0.f));
    }
}
