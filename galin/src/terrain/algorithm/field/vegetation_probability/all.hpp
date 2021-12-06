#pragma once

#include "terrain/all.hpp"

inline 
void update_vegetation_density(Terrain& t) {
    auto g = value_accessor(t.gradient);
    auto h = value_accessor(t.height);
    auto s = value_accessor(t.slope);
    auto vp = value_accessor(t.vegetation_probability);

    auto n0 = resolution(t)[0];
    auto n1 = resolution(t)[1];
    for(size_t i = 0; i < n0; ++i)
    for(size_t j = 0; j < n1; ++j) {
        auto& vp_ij = vp(i, j);
        { // 0
            
        } { // 1

        } { // 2

        }
    }

    { // Normalize vectors.

    }
}
