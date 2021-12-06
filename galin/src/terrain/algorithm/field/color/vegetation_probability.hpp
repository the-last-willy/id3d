#pragma once

#include "terrain/all.hpp"

inline
void update_color_using_vegetation_probability(Terrain& t) {
    auto c = value_accessor(t.color);
    auto vp = value_accessor(t.vegetation_probability);

    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        auto vp_ij = vp(i, j);
        c(i, j) = agl::vec3(vp_ij[0], vp_ij[1], vp_ij[2]);
    }
}
