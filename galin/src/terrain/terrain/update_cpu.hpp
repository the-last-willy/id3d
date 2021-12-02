#pragma once

#include "terrain.hpp"

inline
void update_cpu(Terrain& t) {
    auto c = value_accessor(t.color);
    auto h = value_accessor(t.height);

    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        position(at(t.vertices, i, j))[1] = h(i, j);
        color(at(t.vertices, i, j)) = c(i, j);
    }
}
