#pragma once

#include "terrain/all.hpp"

inline
void update_colors_using_laplacian(Terrain& t) {
    auto c = value_accessor(t.color);
    auto l = value_accessor(t.laplacian);

    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        auto l_ij = l(i, j);
        c(i, j) = agl::vec3(
            std::max(  l_ij, 0.f),
            0.f,
            std::max(- l_ij, 0.f));
    }
}
