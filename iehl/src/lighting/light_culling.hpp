#pragma once

#include "light_group.hpp"

#include <agl/common/all.hpp>
#include <agl/engine/opengl/all.hpp>

#include <array>
#include <vector>

struct LightSpan {
    GLuint first;
    GLuint count;
};

struct LightCulling {
    agl::common::Interval<agl::Vec3> domain;
    std::array<GLuint, 3> resolution = {1, 1, 1};

    std::vector<GLuint> light_indices;
    std::vector<LightSpan> light_spans;

    agl::engine::opengl::Buffer light_index_ssbo;
    agl::engine::opengl::Buffer light_span_ssbo;
};

inline
auto light_culling(
    const LightGroup& lg,
    agl::common::Interval<agl::Vec3> domain,
    std::array<GLuint, 3> resolution)
{
    auto lc = LightCulling();
    {
        lc.domain = domain;
        lc.resolution = resolution;
    }
    {
        auto step = length(lc.domain);
        step[0] /= float(lc.resolution[0]);
        step[1] /= float(lc.resolution[1]);
        step[2] /= float(lc.resolution[2]);
        for(GLuint i0 = 0; i0 < lc.resolution[0]; ++i0)
        for(GLuint i1 = 0; i1 < lc.resolution[1]; ++i1)
        for(GLuint i2 = 0; i2 < lc.resolution[2]; ++i2) {
            auto cell = agl::common::interval(
                agl::vec3(float(i0), float(i1), float(i2)) * step + lower_bound(lc.domain),
                agl::vec3(float(i0 + 1), float(i1 + 1), float(i2 + 1)) * step + lower_bound(lc.domain));
            auto first = GLuint(size(lc.light_indices));
            auto count = GLuint(0);
            for(GLuint li = 0; li < size(lg.lights); ++li) {
                auto lp = lg.lights[li].position;
                auto d = agl::distance(clamp(cell, lp), lp);
                if(d < 5.f) {
                    lc.light_indices.push_back(li);
                    count += 1;
                }
            }
            lc.light_spans.emplace_back(first, count);
        }
    }
    {
        glNamedBufferStorage(
            lc.light_index_ssbo, 
            size(lc.light_indices) * sizeof(GLuint), 
            data(lc.light_indices),
            GL_NONE);
        glNamedBufferStorage(
            lc.light_span_ssbo, 
            size(lc.light_spans) * sizeof(LightSpan), 
            data(lc.light_spans),
            GL_NONE);
    }
    return lc;
}
