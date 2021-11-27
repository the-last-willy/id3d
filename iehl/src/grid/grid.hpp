#pragma once

#include "scene/scene.hpp"

#include <agl/common/all.hpp>

#include <vector>

struct GridCell {
    agl::common::Interval<agl::Vec3> bounds;

    std::size_t first = 0;
    std::size_t last = 0;
};

inline
bool is_empty(const GridCell& gc) {
    return gc.first == gc.last;
}

struct Grid {
    agl::common::Grid<GridCell> cells;

    std::vector<std::array<unsigned, 3>> triangle_indices;
};

inline
auto grid(const Scene& s, std::size_t resolution) {
    auto g = Grid();
    {
        g.cells = agl::common::grid<GridCell>(
            resolution, resolution, resolution);
        g.triangle_indices = s.triangle_indices;
    }
    {
        auto triangles = std::vector<std::pair<std::size_t, unsigned>>();
        triangles.resize(size(s.triangle_indices));
        auto sb = s.bounds;
        auto m0 = mapping(
            projection(sb, 0),
            agl::common::interval(0.f, float(resolution)));
        auto m1 = mapping(
            projection(sb, 1),
            agl::common::interval(0.f, float(resolution)));
        auto m2 = mapping(
            projection(sb, 2),
            agl::common::interval(0.f, float(resolution)));
        for(std::size_t i = 0; i < size(g.triangle_indices); ++i) {
            auto c = centroid(s, i);
            auto i0 = unsigned(m0(c[0]) - (c[0] >= upper_bound(sb)[0]));
            auto i1 = unsigned(m1(c[1]) - (c[1] >= upper_bound(sb)[1]));
            auto i2 = unsigned(m2(c[2]) - (c[2] >= upper_bound(sb)[2]));
            triangles[i] = std::make_pair(at(indexing(g.cells), i0, i1, i2), unsigned(i));
            // std::cout << c << " = [" << i0 << " " << i1 << " " << i2 << "] = " << at(indexing(g.cells), i0, i1, i2) << std::endl;

        }
        std::sort(begin(triangles), end(triangles),
            [](auto a0, auto a1) {
                return a0.first < a1.first; });
        { // Re-order triangles.
            auto sorted = std::vector<std::array<unsigned, 3>>();
            sorted.resize(size(s.triangle_indices));
            for(std::size_t i = 0; i < size(sorted); ++i) {
                sorted[i] = s.triangle_indices[triangles[i].second];
            }
            g.triangle_indices = std::move(sorted);
        }
        { // Compute cell ranges.
            auto cell_i = triangles.front().first;
            auto first = std::size_t(0);
            for(std::size_t i = 0; i < size(triangles); ++i) {
                auto& tr = triangles[i];
                // std::cout << cell_i << " ";
                if(cell_i != tr.first) {
                    // std::cout << first << " " << i;
                    auto& c = at(g.cells, cell_i);
                    c.first = first;
                    c.last = i;
                    first = i;
                    cell_i = tr.first;
                }
                // std::cout << std::endl;
            }
            {
                auto& c = at(g.cells, cell_i);
                c.first = first;
                c.last = size(triangles);
            }
        }
    }
    { // Compute cell bounds.
        for(auto& c : g.cells) {
            if(not is_empty(c)) {
                c.bounds = agl::common::interval(
                    s.vertex_positions[g.triangle_indices[c.first][0]]);
                for(auto i = c.first; i != c.last; ++i) 
                for(auto ti : g.triangle_indices[i]) {
                    extend(c.bounds, s.vertex_positions[ti]);
                }
            }
        }
    }
    return g;
}
