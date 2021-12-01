#pragma once

inline
void update_normal_color_field(Terrain& t) {
    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        auto n = at(t.normals, i, j);
        at(t.colors, i, j) = 0.5f * n + 0.5f;
    }
}
