#pragma once

#include "terrain/all.hpp"

inline
void update_laplacian(Terrain& t) {
    auto dx = delta(t)[0];
    auto dy = delta(t)[1];

    auto inv_dx2 = 1.f / (dx * dx);
    auto inv_dy2 = 1.f / (dy * dy);

    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];

    auto h = value_accessor(t.height);
    auto l = value_accessor(t.laplacian);

    auto inf0 = 0;
    auto sup0 = nx - 1;
    auto inf1 = 0;
    auto sup1 = ny - 1;

    // Interior.

    for(std::size_t i = 1; i < sup0; ++i)
    for(std::size_t j = 1; j < sup1; ++j) {
        auto dfdx = (h(i - 1, j) - 2 * h(i, j) + h(i + 1, j)) * inv_dx2;
        auto dfdy = (h(i, j - 1) - 2 * h(i, j) + h(i, j + 1)) * inv_dy2;
        l(i, j) = dfdx + dfdy;
    }

    // Borders.

    { // Inf, j.
        auto i = inf0;
        for(size_t j = 1; j < sup1; ++j) {
            auto hij = h(i, j);
            auto dfdx = (            -     hij + h(i + 1, j    )) * inv_dx2;
            auto dfdy = (h(i, j - 1) - 2 * hij + h(i    , j + 1)) * inv_dy2;
            l(i, j) = dfdx + dfdy;
        }
    }
    { // Sup, j.
        auto i = sup0;
        for(size_t j = 1; j < sup1; ++j) {
            auto hij = h(i, j);
            auto dfdx = (h(i - 1, j    ) -     hij              ) * inv_dx2;
            auto dfdy = (h(i    , j - 1) - 2 * hij + h(i, j + 1)) * inv_dy2;
            l(i, j) = dfdx + dfdy;
        }
    }
    { // i, inf.
        auto j = inf1;
        for(size_t i = 1; i < sup0; ++i) {
            auto hij = h(i, j);
            auto dfdx = (h(i - 1, j) - 2 * hij + h(i + 1, j    )) * inv_dx2;
            auto dfdy = (            -     hij + h(i    , j + 1)) * inv_dy2;
            l(i, j) = dfdx + dfdy;
        }
    }
    { // i, sup.
        auto j = sup1;
        for(size_t i = 1; i < sup0; ++i) {
            auto hij = h(i, j);
            auto dfdx = (h(i - 1, j    ) - 2 * hij + h(i + 1, j)) * inv_dx2;
            auto dfdy = (h(i    , j - 1) -     hij              ) * inv_dy2;
            l(i, j) = dfdx + dfdy;
        }
    }

    // Corners.

    { // Inf, inf.
        auto i = inf0;
        auto j = inf1;
        auto hij = h(i, j);
        auto dfdx = (                - hij + h(i + 1, j    )) * inv_dx2;
        auto dfdy = (                - hij + h(i    , j + 1)) * inv_dy2;
        l(i, j) = dfdx + dfdy;
    }
    { // Inf, sup.
        auto i = inf0;
        auto j = sup1;
        auto hij = h(i, j);
        auto dfdx = (                - hij + h(i + 1, j    )) * inv_dx2;
        auto dfdy = (h(i    , j - 1) - hij                  ) * inv_dy2;
        l(i, j) = dfdx + dfdy;
    }
    { // Sup, inf.
        auto i = sup0;
        auto j = inf1;
        auto hij = h(i, j);
        auto dfdx = (h(i - 1, j    ) - hij                  ) * inv_dx2;
        auto dfdy = (                - hij + h(i    , j + 1)) * inv_dy2;
        l(i, j) = dfdx + dfdy;
    }
    { // Sup, sup.
        auto i = sup0;
        auto j = sup1;
        auto hij = h(i, j);
        auto dfdx = (h(i - 1, j    ) - hij) * inv_dx2;
        auto dfdy = (h(i    , j - 1) - hij) * inv_dy2;
        l(i, j) = dfdx + dfdy;
    }
}
