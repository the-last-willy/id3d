#pragma once

#include "terrain/all.hpp"

#include <cmath>

inline
void update_colors_using_laplacian(Terrain& t) {
    auto c = value_accessor(t.color);
    auto l = value_accessor(t.laplacian);

    auto compressed = [](float f) {
        return std::sqrt(std::abs(f) + 1.f) - 1.f;
    };

    auto min = lower_bound(t.laplacian.range);
    auto max = upper_bound(t.laplacian.range);
    auto extent = std::max(compressed(min), compressed(max));

    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        auto l_ij = l(i, j);
        auto cl = compressed(l_ij);

        auto r = 0.f;
        auto b = 0.f;
        if(l_ij > 0.f) {
            r = cl / extent;
        } else if(l_ij < 0.f) {
            b = cl / extent;
        }

        c(i, j) = agl::vec3(r, 0.f, b);
    }
}
