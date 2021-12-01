#pragma once

#include "resolution.hpp"
#include "terrain.hpp"

// inline
// void update_colors(Terrain& t) {
//     for(std::size_t i = 0; i < resolution(t)[0]; ++i)
//     for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
//         auto l = at(t.laplacian, i, j);
//         at(t.colors, i, j)
//         = agl::vec3(
//             std::min(std::max(l, 0.f), 1.f),
//             0.f,
//             std::min(std::max(-l, 0.f), 1.f));
//     }
// }

inline
void update_colors(Terrain& t) {
    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        auto l = at(t.drainage_areas, i, j);
        at(t.colors, i, j)
        = agl::vec3(1 / (l / 100.f + 1));
    }
}
