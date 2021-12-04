#pragma once

#include "terrain/all.hpp"

inline
void stream_power_erosion(Terrain &t, float extent) {
    auto da = value_accessor(t.drainage_area);
    auto h = value_accessor(t.height);
    auto s = value_accessor(t.slope);

    auto factor = extent / (
        upper_bound(t.slope.range)
        * std::sqrt(upper_bound(t.drainage_area.range)));

    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];
    for(size_t i = 0; i < nx; ++i)
    for(size_t j = 0; j < ny; ++j) {
        h(i, j) -= factor * s(i, j) * std::sqrt(da(i, j));
    }
}
