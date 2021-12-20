#pragma once

#include "light/all.hpp"

#include <agl/engine/all.hpp>

#include <array>
#include <vector>

struct LightSpan {
    GLuint first;
    GLuint count;
};

struct LightGrid {
    agl::common::Interval<agl::Vec3> domain;
    std::array<GLuint, 3> resolution = {1, 1, 1};

    std::vector<GLuint> light_indices;
    std::vector<LightSpan> light_spans;

    agl::engine::opengl::Buffer light_index_ssbo;
    agl::engine::opengl::Buffer light_span_ssbo;
};

inline
auto light_grid(
    std::span<const Light> lights,
    agl::common::Interval<agl::Vec3> domain,
    std::array<GLuint, 3> resolution)
{
    auto lg = LightGrid();
    {
        lg.domain = domain;
        lg.resolution = resolution;
    }
    {
        auto step = length(lg.domain);
        step[0] /= float(lg.resolution[0]);
        step[1] /= float(lg.resolution[1]);
        step[2] /= float(lg.resolution[2]);
        for(GLuint i0 = 0; i0 < lg.resolution[0]; ++i0)
        for(GLuint i1 = 0; i1 < lg.resolution[1]; ++i1)
        for(GLuint i2 = 0; i2 < lg.resolution[2]; ++i2) {
            auto cell = agl::common::interval(
                agl::vec3(float(i0), float(i1), float(i2)) * step + lower_bound(lg.domain),
                agl::vec3(float(i0 + 1), float(i1 + 1), float(i2 + 1)) * step + lower_bound(lg.domain));
            auto first = GLuint(size(lg.light_indices));
            auto count = GLuint(0);
            for(GLuint li = 0; li < size(lights); ++li) {
                auto lp = lights[li].position;
                auto d = agl::distance(clamp(cell, lp), lp);
                if(d < 5.f) {
                    lg.light_indices.push_back(li);
                    count += 1;
                }
            }
            lg.light_spans.emplace_back(first, count);
        }
    }
    {
        glNamedBufferStorage(
            lg.light_index_ssbo, size(lg.light_indices) * sizeof(GLuint), data(lg.light_indices), 0);
        glNamedBufferStorage(
            lg.light_span_ssbo, size(lg.light_spans) * sizeof(LightSpan), data(lg.light_spans), 0);
    }
    return lg;
}
