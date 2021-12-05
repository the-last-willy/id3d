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
    auto neighbours4 = std::vector<agl::Vec2> {
        agl::vec2(0, 1), agl::vec2(0, -1), agl::vec2(-1, 0), agl::vec2(1, 0)
    };
    auto neighbours_diag = std::vector<agl::Vec2> {
        agl::vec2(-1, 1), agl::vec2(1, 1), agl::vec2(-1, -1), agl::vec2(1, -1)
    };

    auto sum = 0.f;
    for(size_t i = 1; i < nx - 1; ++i) {
        for(size_t j = 1; j < ny - 1; ++j) {
            auto current_height = h(i, j);

            for(auto& n: neighbours4) {
                if(n[0] != 0)
                    sum += std::abs((current_height - h(i + n[0], j + n[1])) / dx);
                else if(n[1] != 0)
                    sum += std::abs((current_height - h(i + n[0], j + n[1])) / dy);
            }
            for(auto& n: neighbours_diag) {
                sum += std::abs((current_height - h(i + n[0], j + n[1])) / sqrt(dx * dx + dy * dy));
            }
            s(i, j) = sum / 8.f;
            sum = 0.f;
        }
    }
}
