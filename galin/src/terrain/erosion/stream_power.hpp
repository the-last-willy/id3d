#pragma once

#include "terrain/all.hpp"

struct StreamPowerErosionSettings {
    float factor = 1.f;
};

inline
void stream_power_erosion(Terrain &t) {
    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];
    for(size_t i = 0; i < nx; ++i)
    for(size_t j = 0; j < ny; ++j) {
        auto da = at(t.drainage_areas, i, j);
        auto s = at(t.slopes, i, j);
        at(t.heights, i, j) += -0.01f * s * std::sqrt(da);
    }
}
