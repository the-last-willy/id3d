#pragma once

#include "terrain/all.hpp"

inline
void update_color_using_vegetation_probability(Terrain& t) {
    auto c = value_accessor(t.color);
    auto vp = value_accessor(t.vegetation_probability);

    for(std::size_t i0 = 0; i0 < resolution(t)[0]; ++i0)
    for(std::size_t i1 = 0; i1 < resolution(t)[1]; ++i1) {
        c(i, j) = 
    }
}
