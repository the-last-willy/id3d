#pragma once

#include "terrain/all.hpp"

inline 
void update_vegetation_probability(Terrain& t) {
    auto g = value_accessor(t.gradient);
    auto h = value_accessor(t.height);
    auto l = value_accessor(t.laplacian);
    auto s = value_accessor(t.slope);
    auto vp = value_accessor(t.vegetation_probability);
    auto w = value_accessor(t.wetness);

    auto n0 = resolution(t)[0];
    auto n1 = resolution(t)[1];
    for(size_t i = 0; i < n0; ++i)
    for(size_t j = 0; j < n1; ++j) {
        auto& vp_ij = vp(i, j);
        { // 0
            vp_ij[0] = std::max(1.f - std::sqrt(2.f * s(i, j)), 0.f);
            vp_ij[0] = std::min(vp_ij[0], 1.f - w(i, j) / upper_bound(t.wetness.range));
            vp_ij[0] = std::min(vp_ij[0], 1.f - (h(i, j) - lower_bound(t.height.range)) / length(t.height.range));
        } { // 1
            vp_ij[1] = std::max(1.f - std::pow(2.f * s(i, j), 1.f / 4.f), 0.f);
            vp_ij[1] = std::min(vp_ij[1], (h(i, j) - lower_bound(t.height.range)) / length(t.height.range));
        } { // 2
            vp_ij[2] = w(i, j) / upper_bound(t.wetness.range);
        }
    }

    { // Normalize vectors.
        for(auto& vp_ij : t.vegetation_probability.values) {
            auto sum = 0.f;
            auto prod = 1.f;
            for(auto p : vp_ij) {
                sum += p;
                prod *= (1.f - p);
            }
            for(auto& p : vp_ij) {
                p = (p / sum) * (1.f - prod);
            }
        }
    }
}
