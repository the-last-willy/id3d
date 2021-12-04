#pragma once

#include "terrain/all.hpp"

inline
void update_drainage_area_using_weighted(Terrain& t) {
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
