#pragma once

#include "terrain/all.hpp"

struct DebrisSlopeErosionSettings {
    float factor = 1.f;
};

inline 
void debris_slope_erosion(Terrain &t) {
    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];
    for(size_t i = 0; i < nx; ++i)
    for(size_t j = 0; j < ny; ++j) {
        auto s = at(t.slopes, i, j);
        at(t.heights, i, j) += -0.01f * s;
    }
}