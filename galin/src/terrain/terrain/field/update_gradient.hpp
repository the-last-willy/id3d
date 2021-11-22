#pragma once

#include "delta.hpp"
#include "resolution.hpp"
#include "terrain.hpp"

inline
void update_gradient(Terrain& t) {
    auto dx = delta(t)[0];
    auto dy = delta(t)[1];
    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];
    auto g = [&](auto i, auto j) -> auto& { return at(t.gradients, i, j); };
    auto h = [&](auto i, auto j) -> auto& { return at(t.heights, i, j); };
    for(std::size_t i = 1; i < nx - 2; ++i) {
        for(std::size_t j = 1; j < ny; ++j) {
            auto dfdx = (h(i + 1, j) - h(i - 1, j)) / (2 * dx);
            auto dfdy = (h(i, j + 1) - h(i, j - 1)) / (2 * dy);
            g(i, j) = agl::vec2(dfdx, dfdy);
        }
    }
    for(size_t j = 1; j < ny - 2; ++j) {
        //replace 0 by domain's lower bound
        auto left_dfdx = (h(1, j) - h(0, j)) / dx;
        auto right_dfdx = (h(nx - 1, j) - h(nx - 2, j)) / dx;
        auto left_dfdy = (h(0, j + 1) - h(0, j - 1)) / (2 * dy);
        auto right_dfdy = (h(nx - 1, j + 1) - h(nx - 1, j - 1)) / (2 * dy);
        g(0, j) = agl::vec2(left_dfdx, left_dfdy);
        g(nx - 1, j) = agl::vec2(right_dfdx, right_dfdy);
    }
    for(size_t i = 1; i < nx - 2; ++i) {
        //replace 0 by domain's lower bound
        auto up_dfdx = (h(i + 1, ny - 1) - h(i - 1, ny - 1)) / (2 * dx);
        auto down_dfdx = (h(i + 1, 0) - h(i - 1, 0)) / (2 * dx);
        auto up_dfdy = (h(1, ny - 1) - h(0, ny - 2)) / dy;
        auto down_dfdy = (h(i, 1) - h(i, 0)) / dy;
        g(i, ny - 1) = agl::vec2(up_dfdx, up_dfdy);
        g(i, 0) = agl::vec2(down_dfdx, down_dfdy);
    }
    // corner
    g(0, 0) = agl::vec2(
        (h(1, 0) - h(0, 0)) / dx,
        (h(0, 1) - h(0, 0)) / dy
    );
    g(nx - 1, 0) = agl::vec2(
        (h(nx - 1, 0) - h(nx - 2, 0)) / dx,
        (h(nx - 1, 1) - h(nx - 1, 0)) / dy
    );
    g(0, ny - 1) = agl::vec2(
        (h(1, ny - 1) - h(0, ny - 1)) / dx,
        (h(0, ny - 1) - h(0, ny - 2)) / dy
    );
    g(nx - 1, ny - 1) = agl::vec2(
        (h(nx - 1, ny - 1) - h(nx - 2, ny - 1)) / dx,
        (h(nx - 1, ny - 1) - h(nx - 1, ny - 2)) / dy
    );
}
