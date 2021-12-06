#pragma once

#include "terrain/all.hpp"

inline 
void update_wetness(Terrain& t) {
    auto da = value_accessor(t.drainage_area);
    auto s = value_accessor(t.slope);
    auto w = value_accessor(t.wetness);
    auto epsilon = 0.01f;

    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];
    for(size_t i = 0; i < nx; ++i)
    for(size_t j = 0; j < ny; ++j) {
        w(i, j) = std::log(da(i, j) / (s(i, j) + epsilon));
    }
}