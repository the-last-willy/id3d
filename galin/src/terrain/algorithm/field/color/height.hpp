#pragma once

#include "terrain/all.hpp"

inline
void update_color_using_height(Terrain& t) {
    auto c = value_accessor(t.color);
    auto h = value_accessor(t.height);

    auto a = length(t.height.range);
    auto b = lower_bound(t.height.range);

    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        c(i, j) = agl::vec3((h(i, j) - b) / a);
    }
}
