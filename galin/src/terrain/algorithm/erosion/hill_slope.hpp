#pragma once

#include "terrain/all.hpp"

struct HillSlopeErosionSettings {
    float factor = 1.f;
};

inline 
void hill_slope_erosion(Terrain &t) {
    auto h = value_accessor(t.height);
    auto l = value_accessor(t.laplacian);

    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];
    for(size_t i = 0; i < nx; ++i)
    for(size_t j = 0; j < ny; ++j) {
        auto laplacian = l(i, j);
        h(i, j) += 0.2f * laplacian;
    }
}