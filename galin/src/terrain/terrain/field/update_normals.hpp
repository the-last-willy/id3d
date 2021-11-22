#pragma once

#include "terrain/all.hpp"

inline 
void update_normals(Terrain &t) {
    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];
    for(size_t i = 0; i < nx; ++i)
    for(size_t j = 0; j < ny; ++j) {
        auto gradient = at(t.gradients, i, j);
        at(t.normals, i, j)
        = agl::normalize(agl::vec3(-gradient, 1));
    }
}