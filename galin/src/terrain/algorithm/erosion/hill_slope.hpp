#pragma once

#include "terrain/all.hpp"

inline 
void hill_slope_erosion(Terrain &t, float extent) {
    auto h = value_accessor(t.height);
    auto l = value_accessor(t.laplacian);

    auto factor = extent / std::max(
        -lower_bound(t.laplacian.range),
        upper_bound(t.laplacian.range));

    auto r0 = resolution(t)[0];
    auto r1 = resolution(t)[1];
    for(size_t i = 0; i < r0; ++i)
    for(size_t j = 0; j < r1; ++j) {
        h(i, j) += factor * l(i, j);
    }
}