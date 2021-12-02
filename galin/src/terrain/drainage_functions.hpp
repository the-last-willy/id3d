#pragma once

#include "terrain/all.hpp"

void steepest_compute(Terrain &t) {
    auto da = value_accessor(t.drainage_area);
    auto h = value_accessor(t.height);

    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];

    for(auto& d : t.drainage_area.values) {
        d = 1.f;
    }
    std::vector<Drainage_data> positions;
    positions.reserve((nx - 2) * (ny - 2));
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
            da(x_max, y_max) += da(x, y);
        }
    }
}

void mean_compute(Terrain &t) {
    auto da = value_accessor(t.drainage_area);
    auto h = value_accessor(t.height);
    
    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];

    for(auto& d : t.drainage_areas) {
        d = 1.f;
    }
    std::vector<Drainage_data> positions;
    positions.reserve((nx - 2) * (ny - 2));
    for(size_t i = 1; i < nx - 1; ++i)
    for(size_t j = 1; j < ny - 1; ++j) {
        positions.push_back({std::array<size_t, 2>{i, j}, h(i, j)});
    }
    std::sort(positions.begin(), positions.end(), compare_height);

    std::vector<std::array<size_t, 2>> neighbours;
    neighbours.reserve(9);
    std::vector<float> distances;
    distances.reserve(9);
    for(std::size_t i = 0; i < positions.size(); ++i) {
        auto x = positions[i].position[0];
        auto y = positions[i].position[1];
        auto current_height = positions[i].height;
        auto total = 0.f;
        for(std::size_t c = x - 1; c <= x + 1; ++c)
        for(std::size_t r = y - 1; r <= y + 1; ++r) {
            auto distance = current_height - h(c, r);
            if(distance > 0) {
                total += distance;
            }
            distances.push_back(distance);
            neighbours.push_back(std::array<size_t, 2>{c, r});
        }
        if(total > 0) {
            for(int j = 0; j < neighbours.size(); ++j) {
                if(distances[j] > 0) {
                    auto w = distances[j] / total;
                    auto p = neighbours[j];
                    da(p[0], p[1]) += w * da(x, y);
                }
            }
        }
        neighbours.clear();
        distances.clear();
    }
}