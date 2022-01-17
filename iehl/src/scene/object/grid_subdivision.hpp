#pragma once

#include "scene/vertex_attribute/vertex_attribute_group.hpp"
#include "object_group.hpp"
#include "triangle_centroid.hpp"

#include <agl/common/all.hpp>

#include <vector>


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
            for(GLuint i = c.firstIndex; i < c.firstIndex + c.count; i += 3) {
                extend(bounds, positions[tis[i / 3][0]]);
                extend(bounds, positions[tis[i / 3][1]]);
                extend(bounds, positions[tis[i / 3][2]]);
            }
            og.data.object_bounds.push_back(agl::common::interval(
                agl::vec4(lower_bound(bounds), 1.f),
                agl::vec4(upper_bound(bounds), 1.f)));
        }
        gl::NamedBufferStorage(og.data.object_bounds_ssbo,
            og.data.object_bounds);
    }
}
