#pragma once

#include "terrain/all.hpp"

inline 
void update_slope_using_mean(Terrain& t) {
    auto h = value_accessor(t.height);
    auto s = value_accessor(t.slope);

    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];
    
    auto dx = delta(t)[0];
    auto dy = delta(t)[1];
    auto dd = std::sqrt(dx * dx + dy * dy);

    for(size_t i = 0; i < nx; ++i) {
        s(i, 0     ) = 0.f;
        s(i, ny - 1) = 0.f;
    }

    for(size_t j = 0; j < ny; ++j) {
        s(0     , j) = 0.f;
        s(ny - 1, j) = 0.f;
    }

    for(size_t i = 1; i < nx - 1; ++i) 
    for(size_t j = 1; j < ny - 1; ++j) {
        auto h_ij = h(i, j);
        auto sum = 0.f;

        sum += std::abs(h(i - 1, j    ) - h_ij) / dx;
        sum += std::abs(h(i + 1, j    ) - h_ij) / dx;

        sum += std::abs(h(i    , j - 1) - h_ij) / dy;
        sum += std::abs(h(i    , j + 1) - h_ij) / dy;

        sum += std::abs(h(i - 1, j - 1) - h_ij) / dd;
        sum += std::abs(h(i - 1, j + 1) - h_ij) / dd;

        sum += std::abs(h(i + 1, j - 1) - h_ij) / dd;
        sum += std::abs(h(i + 1, j + 1) - h_ij) / dd;

        s(i, j) = sum / 8.f;
    }
}
