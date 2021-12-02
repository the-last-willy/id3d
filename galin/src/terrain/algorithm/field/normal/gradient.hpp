#pragma once

#include "terrain/all.hpp"

inline 
void update_normals_using_gradient(Terrain &t) {
    auto g = value_accessor(t.gradient);
    auto n = value_accessor(t.normal);

    auto r0 = resolution(t)[0];
    auto r1 = resolution(t)[1];
    for(size_t i = 0; i < r0; ++i)
    for(size_t j = 0; j < r1; ++j) {
        n(i, j) = agl::normalize(agl::vec3(-g(i, j), 1.f));
    }
}
