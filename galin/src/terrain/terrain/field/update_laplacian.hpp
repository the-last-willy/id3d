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
    for(std::size_t i = 1; i < nx - 2; ++i) {
        for(std::size_t j = 1; j < ny - 2; ++j) {
            auto dfdx = (h(i - 1, j) - 2 * h(i, j) + h(i + 1, j)) / (dx * dx);
            auto dfdy = (h(i, j - 1) - 2 * h(i, j) + h(i, j + 1)) / (dy * dy);
            l(i, j) = dfdx + dfdy;
        }
    }
    // for(size_t j = 1; j < ny - 2; ++j) {
    //     auto left_dfdx = (h(0, j) - 2 * h(0, j) + h(1, j)) / (dx * dx);
    //     auto left_dfdy = (h(nx - 2, j - 1) - h(nx - 1, j) + h(nx - 1, j + 1)) / (dy * dy);
    //     auto right_dfdx = (h(0, j) - 2 * h(0, j) + h(1, j)) / (dx * dx);
    //     auto right_dfdy = (h(nx - 1, j - 1) - h(nx - 1, j) + h(nx - 1, j + 1)) / (dy * dy);
    //     l(0, j) = left_dfdx + left_dfdy;
    //     l(nx - 1, j) = right_dfdx + right_dfdy;
    // }
    // for(size_t i = 1; i < nx - 2; ++i) {
    //     auto up_dfdx = (h(i - 1, ny - 1) - 2 * h(i, ny - 1) + h(i + 1, ny - 1)) / (dx * dx);
    //     auto up_dfdy = (h(i, ny - 2) - h(i, ny - 1) + h(i, ny - 1)) / (dy * dy);
    //     auto down_dfdx = (h(i - 1, ny - 1) - 2 * h(i, ny - 1) + h(i + 1, ny - 1)) / (dx * dx);
    //     auto down_dfdy = (h(i, 0) - h(i, 0) + h(i, 1)) / (dy * dy);
    //     l(i, ny - 1) = up_dfdx + up_dfdy;
    //     l(i, 0) = down_dfdx + down_dfdy;
    // }
}
