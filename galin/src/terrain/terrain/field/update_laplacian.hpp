#pragma once

#include "delta.hpp"
#include "resolution.hpp"
#include "terrain.hpp"

inline
void update_laplacian(Terrain& t) {
    auto dx = delta(t)[0];
    auto dy = delta(t)[1];
    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];
    auto h = [&](auto i, auto j) -> auto& { return at(t.heights, i, j); };
    auto l = [&](auto i, auto j) -> auto& { return at(t.laplacian, i, j); };
    auto inv_dx2 = 1.f / (dx * dx);
    auto inv_dy2 = 1.f / (dy * dy);
    for(std::size_t i = 1; i < nx - 1; ++i) {
        for(std::size_t j = 1; j < ny - 1; ++j) {
            auto dfdx = (h(i - 1, j) - 2 * h(i, j) + h(i + 1, j)) * inv_dx2;
            auto dfdy = (h(i, j - 1) - 2 * h(i, j) + h(i, j + 1)) * inv_dy2;
            l(i, j) = dfdx + dfdy;
        }
    }
    for(size_t j = 1; j < ny - 1; ++j) {
        {
            auto dfdx = (            -     h(0, j) + h(1, j    )) * inv_dx2;
            auto dfdy = (h(0, j - 1) - 2 * h(0, j) + h(0, j + 1)) * inv_dy2;
            l(0, j) = dfdx + dfdy;
        }
        {
            auto dfdx = (h(nx - 2, j    ) -     h(nx - 1, j)                   ) * inv_dx2;
            auto dfdy = (h(nx - 1, j - 1) - 2 * h(nx - 1, j) + h(nx - 1, j + 1)) * inv_dy2;
            l(nx - 1, j) = dfdx + dfdy;
        }
    }
    for(size_t i = 1; i < nx - 1; ++i) {
        {
            auto dfdx = (h(i - 1, 0) - 2 * h(i, 0) + h(i + 1, 0)) * inv_dx2;
            auto dfdy = (            -     h(i, 0) + h(i    , 1)) * inv_dy2;
            l(i, 0) = dfdx + dfdy;
        }
        {
            auto dfdx = (h(i - 1, ny - 1) - 2 * h(i, ny - 1) + h(i + 1, ny - 1)) * inv_dx2;
            auto dfdy = (h(i    , ny - 2) -     h(i, ny - 1)                   ) * inv_dy2;
            l(i, ny - 1) = dfdx + dfdy;
        }
    }
    {

    }
}
