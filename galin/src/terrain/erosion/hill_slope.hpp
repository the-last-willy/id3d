#pragma once

#include "terrain/all.hpp"

struct HillSlopeErosionSettings {
    float factor = 1.f;
};

inline 
void hill_slope_erosion(Terrain &t) {
    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];
    for(size_t i = 0; i < nx; ++i)
    for(size_t j = 0; j < ny; ++j) {
        auto laplacian = at(t.laplacian, i, j);
        at(t.heights, i, j) += 0.2f * laplacian;
    }
}