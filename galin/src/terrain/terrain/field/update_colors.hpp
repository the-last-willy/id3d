#pragma once

#include "resolution.hpp"
#include "terrain.hpp"

inline
void update_colors(Terrain& t) {
    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        at(t.colors, i, j)
        = agl::vec3(1 / (at(t.laplacian, i, j) + 1));
    }
}
