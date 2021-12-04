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
        // auto up = h(x, y + 1);
        // auto down = h(x, y - 1);
        // auto left = h(x - 1, y);
        // auto right = h(x + 1, y);
        // auto current_height = positions[i].height;
        // auto max = current_height - up;
        // auto x_max = x;
        // auto y_max = y + 1;

        // if(max < current_height - down) {
        //     max = current_height - down;
        //     x_max = x;
        //     y_max = y - 1;
        // }
        // if (max < current_height - left) {
        //     max = current_height - left;
        //     x_max = x - 1;
        //     y_max = y;
        // }
        // if (max < current_height - right) {
        //     max = current_height - right;
        //     x_max = x + 1;
        //     y_max = y;
        // }
        // da(x_max, y_max) += da(x, y);

        auto current_height = positions[i].height;
        auto max = current_height - h(x - 1, y + 1);
        auto x_max = x - 1;
        auto y_max = y + 1;
        for(std::size_t c = x - 1; c <= x + 1; ++c) {
            for(std::size_t r = y - 1; r <= y + 1; ++r) {
                if(r != x && c != y) {
                    auto distance = current_height - h(c, r);
                    if(max < distance) {
                        max = distance;
                        x_max = c;
                        y_max = r;
                    }
                }
            }
        }
        da(x_max, y_max) += da(x, y);
    }
}
