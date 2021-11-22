#pragma once

#include "resolution.hpp"
#include "terrain.hpp"

inline 
void update_slope(Terrain& t) {
    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];
    for(size_t i = 0; i < nx; ++i)
    for(size_t j = 0; j < ny; ++j) {
        auto gradient = at(t.gradients, i, j);
        at(t.slopes, i, j) = agl::length(gradient);
    }
}
