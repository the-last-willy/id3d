#pragma once

#include "terrain/all.hpp"

inline 
void update_slope_using_gradient(Terrain& t) {
    auto g = value_accessor(t.gradient);
    auto s = value_accessor(t.slope);

    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];
    for(size_t i = 0; i < nx; ++i)
    for(size_t j = 0; j < ny; ++j) {
        s(i, j) = agl::length(g(i, j));
    }
}
