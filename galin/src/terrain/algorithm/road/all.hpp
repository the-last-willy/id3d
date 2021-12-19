#pragma once

#include <list>
#include <set>
#include <utility>
#include <algorithm>
#include <iterator>

#include "terrain/terrain.hpp"

auto equal(agl::Vec2 a, agl::Vec2 b) {
    return a[0] == b[0] && a[1] == b[1];
}

auto cost(float d, float s, float k) {
    return (1 - k) * d + k * s;
}

inline
auto compute_path(Terrain& t, agl::Vec2& start, agl::Vec2& end) {
    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];
    auto previous = std::vector<agl::Vec2>(nx * ny);
    auto min_distance = std::vector<float>(nx * ny, max_weight);
    auto source_index = static_cast<int>(ny * start[0] + start[1]);
    min_distance[source_index] = 0.f; 

    auto cmp = [](const std::pair<float, agl::Vec2>& a,
                          const std::pair<float, agl::Vec2>& b)
    {
        if(a.first != b.first)
            return a.first < b.first;
        else if(a.second[0] != b.second[0])
            return a.second[0] < b.second[0];
        else if(a.second[1] != b.second[1])
            return a.second[1] < b.second[1];
        else
            return false;
    };

    std::set<std::pair<float, agl::Vec2>, decltype(cmp)> queue(cmp);
    queue.insert(std::make_pair(min_distance[source_index], start));
    while(!queue.empty()) {
        auto dist = queue.begin()->first;
        auto u = queue.begin()->second;
        queue.erase(queue.begin());
        const auto& neighbours = at(t.graph, static_cast<int>(u[0]), static_cast<int>(u[1]));
        for(auto it = neighbours.begin(); it != neighbours.end(); ++it) {
            auto weight = it->weight;
            auto target = it->target;
            auto distance_through_u = dist + weight;
            auto i = nx * static_cast<int>(target[0]) + static_cast<int>(target[1]);
            if(distance_through_u < min_distance[i]) {
                queue.erase(std::make_pair(min_distance[i], target));
                min_distance[i] = distance_through_u;
                previous[i] = u;
                queue.insert(std::make_pair(min_distance[i], target));
            }
        } 
    }

    auto path = std::list<agl::Vec2>();
    for(agl::Vec2 v = end; !equal(v, start);
         v = previous[ny * static_cast<int>(v[0]) + static_cast<int>(v[1])])
    {
        path.push_front(v);
    }
    return path;
}


inline
void update_road(Terrain& t) {
    auto col = value_accessor(t.color);
    auto h = value_accessor(t.height);

    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];

    auto sup0 = nx - 1;
    auto sup1 = ny - 1;

    auto dx = delta(t)[0];
    auto dy = delta(t)[1];
    auto dd = std::sqrt(dx * dx + dy * dy);

    auto k = 1.f;

    for(std::size_t i = 1; i < sup0; ++i)
    for(std::size_t j = 1; j < sup1; ++j) {
        auto h_ij = h(i, j);
        auto& n = at(t.graph, i, j);
        n.push_back(neighbour{agl::vec2(i - 1, j),
        cost(dx, std::abs(h(i - 1, j    ) - h_ij) / dx, k)});
        n.push_back(neighbour{agl::vec2(i + 1, j),
        cost(dx, std::abs(h(i + 1, j    ) - h_ij) / dx, k)});

        n.push_back(neighbour{agl::vec2(i, j - 1),
        cost(dy, std::abs(h(i    , j - 1) - h_ij) / dy, k)});
        n.push_back(neighbour{agl::vec2(i, j + 1),
        cost(dy, std::abs(h(i    , j + 1) - h_ij) / dy, k)});

        n.push_back(neighbour{agl::vec2(i - 1, j - 1),
        cost(dd, std::abs(h(i - 1, j - 1) - h_ij) / dd, k)});
        n.push_back(neighbour{agl::vec2(i - 1, j + 1),
        cost(dd, std::abs(h(i - 1, j + 1) - h_ij) / dd, k)});

        n.push_back(neighbour{agl::vec2(i + 1, j - 1),
        cost(dd, std::abs(h(i + 1, j - 1) - h_ij) / dd, k)});
        n.push_back(neighbour{agl::vec2(i + 1, j + 1),
        cost(dd, std::abs(h(i + 1, j + 1) - h_ij) / dd, k)});
    }


    auto start = agl::vec2(50, 10);
    auto end = agl::vec2(250, 200);
    auto path = compute_path(t, start, end);

    for(auto& p : path) {
        col(p[0], p[1]) = agl::vec3(1.f, 0.f, 0.f);
    }

    // auto s2 = agl::vec2(10, 100);
    // auto e2 = agl::vec2(280, 20);
    // auto p2 = compute_path(t, s2, e2);

    // for(auto& p : p2) {
    //     col(p[0], p[1]) = agl::vec3(1.f, 0.f, 0.f);
    // }
}