#pragma once

#include "vertex_attribute/vertex_attribute_group.hpp"
#include "object_group.hpp"
#include "triangle_centroid.hpp"

#pragma once

#include "scene/scene.hpp"

#include <agl/common/all.hpp>

#include <vector>

// struct GridCell {
//     agl::common::Interval<agl::Vec3> bounds;

//     std::size_t first = 0;
//     std::size_t last = 0;
// };

// inline
// bool is_empty(const GridCell& gc) {
//     return gc.first == gc.last;
// }

// struct Grid {
//     agl::common::Grid<GridCell> cells;
//     std::size_t non_empty_cell_count = 0;

//     std::vector<std::array<unsigned, 3>> triangle_indices;
//     std::vector<unsigned> triangle_arrangement;
// };

// inline
// auto grid(const Scene& s, std::size_t resolution) {
//     auto g = Grid();
//     {
//         // g.cells = agl::common::grid<GridCell>(
//         //     resolution, resolution, resolution);
//         // g.triangle_indices = s.objects.topology.triangle_indices;
//     }
//     {
//         // auto triangles = std::vector<std::pair<std::size_t, unsigned>>();
//         // triangles.resize(size(s.objects.topology.triangle_indices));
//         // auto sb = s.bounds;
//         // auto m0 = mapping(
//         //     projection(sb, 0),
//         //     agl::common::interval(0.f, float(resolution)));
//         // auto m1 = mapping(
//         //     projection(sb, 1),
//         //     agl::common::interval(0.f, float(resolution)));
//         // auto m2 = mapping(
//         //     projection(sb, 2),
//         //     agl::common::interval(0.f, float(resolution)));
//         for(std::size_t i = 0; i < size(g.triangle_indices); ++i) {
//             auto c = triangle_centroid(s.objects, i);
//             auto i0 = unsigned(m0(c[0]) - (c[0] >= upper_bound(sb)[0]));
//             auto i1 = unsigned(m1(c[1]) - (c[1] >= upper_bound(sb)[1]));
//             auto i2 = unsigned(m2(c[2]) - (c[2] >= upper_bound(sb)[2]));
//             triangles[i] = std::make_pair(at(indexing(g.cells), i0, i1, i2), unsigned(i));
//         }
//         std::sort(begin(triangles), end(triangles),
//             [](auto a0, auto a1) {
//                 return a0.first < a1.first; });
//         // { // Triangle arrangement.
//         //     g.triangle_arrangement.resize(size(triangles));
//         //     for(std::size_t i = 0; i < size(triangles); ++i) {
//         //         g.triangle_arrangement[i] = triangles[i].second;
//         //     }
//         // }
//         { // Re-order triangles.
//             auto sorted = std::vector<std::array<unsigned, 3>>();
//             sorted.resize(size(s.objects.topology.triangle_indices));
//             for(std::size_t i = 0; i < size(sorted); ++i) {
//                 sorted[i] = s.objects.topology.triangle_indices[g.triangle_arrangement[i]];
//             }
//             g.triangle_indices = std::move(sorted);
//         }
//         { // Compute cell ranges.
//             auto cell_i = triangles.front().first;
//             auto first = std::size_t(0);
//             for(std::size_t i = 0; i < size(triangles); ++i) {
//                 auto& tr = triangles[i];
//                 if(cell_i != tr.first) {
//                     auto& c = at(g.cells, cell_i);
//                     c.first = first;
//                     c.last = i;
//                     first = i;
//                     cell_i = tr.first;
//                 }
//             }
//             {
//                 auto& c = at(g.cells, cell_i);
//                 c.first = first;
//                 c.last = size(triangles);
//             }
//         }
//     }
//     { // Compute cell bounds.
//         for(auto& c : g.cells) {
//             if(not is_empty(c)) {
//                 g.non_empty_cell_count += 1;
//                 c.bounds = agl::common::interval(
//                     s.objects.vertex_attributes.positions[g.triangle_indices[c.first][0]]);
//                 for(auto i = c.first; i != c.last; ++i) 
//                 for(auto ti : g.triangle_indices[i]) {
//                     extend(c.bounds, s.objects.vertex_attributes.positions[ti]);
//                 }
//             }
//         }
//     }
//     return g;
// }

inline
void grid_subdivision(
    ObjectGroup& og,
    std::array<GLint, 3> resolution)
{
    std::cout << "--Subdividing geometry." << std::endl;

    using cell_i = std::size_t;
    using triangle_i = GLuint;
    auto indexing = std::vector<std::pair<cell_i, triangle_i>>();
    auto cell_i_less = [](auto a0, auto a1) { return a0.first < a1.first; };

    auto lb = lower_bound(og.data.bounds);
    auto ub = upper_bound(og.data.bounds);
    auto r = resolution;

    // Assign each triangle to a cell.

    for(GLuint ti = 0; ti < size(og.topology.triangle_indices); ++ti) {
        auto c = triangle_centroid(og, ti);
        auto i0 = GLuint((c[0] - lb[0]) / (ub[0] - lb[0]) * r[0]);
        auto i1 = GLuint((c[1] - lb[1]) / (ub[1] - lb[1]) * r[1]);
        auto i2 = GLuint((c[2] - lb[2]) / (ub[2] - lb[2]) * r[2]);

        // Out of bounds handling ?
        // Just makes sure the ub is inside a cell.

        i0 -= (c[0] >= ub[0]);
        i1 -= (c[1] >= ub[1]);
        i2 -= (c[2] >= ub[2]);

        auto linearized = i0 * r[1] * r[2] + i1 * r[2] + i2;
        indexing.emplace_back(linearized, ti);
    }

    // Sort by cell id.
    {
        std::sort(begin(indexing), end(indexing), cell_i_less);
    }

    // Order triangle indices / element buffer.
    {
        auto ordered_ts = std::vector<std::array<GLuint, 3>>(
            size(og.topology.triangle_indices));
        for(std::size_t i = 0; i < size(indexing); ++i) {
            ordered_ts[i] = og.topology.triangle_indices[indexing[i].second];
        }
        og.topology.triangle_indices = std::move(ordered_ts);

        og.topology.element_buffer = gl::Buffer();
        gl::NamedBufferStorage(og.topology.element_buffer,
            std::span(og.topology.triangle_indices));
    }

    // Order triangle data since triangle order changed.
    {
        auto material_ids = std::vector<int>(
            size(og.data.triangle_material_ids));
        for(std::size_t i = 0; i < size(indexing); ++i) {
            material_ids[i] = og.data.triangle_material_ids[indexing[i].second];
        }
        og.data.triangle_material_ids = std::move(material_ids);

        og.data.triangle_material_id_ssbo = gl::Buffer();
        gl::NamedBufferStorage(og.data.triangle_material_id_ssbo,
            std::span(og.data.triangle_material_ids));
    }
    // Make draw commands.
    {
        auto commands = std::vector<gl::DrawElementsIndirectCommand>();
        auto first = GLuint(0);
        for(std::size_t i = 0; i < size(indexing);) {
            auto current_cell = indexing[i].first;
            auto count = GLuint(1);
            for(++i; i < size(indexing); ++i) {
                if(current_cell == indexing[i].first) {
                    count += 1;
                } else {
                    break;
                }
            }
            auto command = commands.emplace_back(gl::DrawElementsIndirectCommand{
                .count = 3 * count,
                .firstIndex = 3 * first,
                .baseInstance = GLuint(size(commands))});
            first += count;
        }

        og.topology.commands.commands = std::move(commands);
        og.topology.commands.commands_buffer = gl::Buffer();
        gl::NamedBufferStorage(og.topology.commands.commands_buffer,
            std::span(og.topology.commands.commands));

        std::cout << "  command count = " << size(og.topology.commands) << std::endl;
    }
    { // Compute bounds.
        auto& positions = og.vertex_attributes.positions;
        auto& tis = og.topology.triangle_indices;

        og.data.object_bounds.clear();
        for(auto& c : og.topology.commands.commands) {
            auto bounds = agl::common::interval(positions[tis[c.firstIndex / 3][0]]);
            for(GLuint i = c.firstIndex; i < c.firstIndex + c.count / 3; ++i) {
                extend(bounds, positions[tis[i / 3][0]]);
                extend(bounds, positions[tis[i / 3][1]]);
                extend(bounds, positions[tis[i / 3][2]]);
            }
            og.data.object_bounds.push_back(agl::common::interval(
                agl::vec4(lower_bound(bounds), 1.f),
                agl::vec4(upper_bound(bounds), 1.f)));
        }
    }
}
