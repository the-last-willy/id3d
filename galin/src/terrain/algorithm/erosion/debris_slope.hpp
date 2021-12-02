#pragma once

#include "terrain/all.hpp"

struct DebrisSlopeErosionSettings {
    float factor = 1.f;
};

inline 
void debris_slope_erosion(Terrain& t) {
    auto h = value_accessor(t.height);
    auto s = value_accessor(t.slope);

    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];
    for(size_t i = 0; i < nx; ++i)
    for(size_t j = 0; j < ny; ++j) {
        h(i, j) += -0.01f * s(i, j);
    }
}