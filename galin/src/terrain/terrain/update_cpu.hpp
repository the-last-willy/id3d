#pragma once

#include "terrain.hpp"

inline
void update_cpu(Terrain& t) {
    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        position(at(t.vertices, i, j))[1] = at(t.heights, i, j);
        color(at(t.vertices, i, j)) = at(t.colors, i, j);
    }
}
