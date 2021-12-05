#pragma once

#include "terrain/all.hpp"

inline
void update_drainage_area_using_steepest(Terrain& t) {
    auto da = value_accessor(t.drainage_area);
    auto h = value_accessor(t.height);

    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];

    for(auto& d : t.drainage_area.values) {
        d = 1.f;
    }

    auto positions = agl::standard::reserved_vector<Drainage_data>((nx - 2) * (ny - 2));

    for(size_t i = 1; i < nx - 1; ++i)
    for(size_t j = 1; j < ny - 1; ++j) {
        positions.push_back({std::array<size_t, 2>{i, j}, h(i, j)});
    }
    
    std::sort(positions.begin(), positions.end(), compare_height);

    for(std::size_t i = 0; i < positions.size(); ++i) {
        auto x = positions[i].position[0];
        auto y = positions[i].position[1];

        auto current_height = positions[i].height;
        auto max = 0.f;
        auto x_max = x;
        auto y_max = y;
        for(std::size_t xi = x - 1; xi <= x + 1; ++xi)
        for(std::size_t yi = y - 1; yi <= y + 1; ++yi) {
            auto distance = current_height - h(xi, yi);
            if(distance > max) {
                max = distance;
                x_max = xi;
                y_max = yi;
            }
        }
        if(x_max != x or y_max != y) {
            da(x_max, y_max) += da(x, y);
        }
    }
}
