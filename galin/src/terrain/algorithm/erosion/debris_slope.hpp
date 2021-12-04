#pragma once

#include "terrain/all.hpp"

inline 
void debris_slope_erosion(Terrain& t, float extent) {
    auto h = value_accessor(t.height);
    auto s = value_accessor(t.slope);

    auto factor = extent / upper_bound(t.slope.range);

    auto r0 = resolution(t)[0];
    auto r1 = resolution(t)[1];
    for(size_t i = 0; i < r0; ++i)
    for(size_t j = 0; j < r1; ++j) {
        h(i, j) -= factor * s(i, j);
    }
}