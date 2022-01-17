#pragma once

#include "light_group.hpp"
#include "opengl.hpp"

#include <agl/common/all.hpp>
#include <agl/standard/all.hpp>

#include <algorithm>
#include <array>
#include <random>
#include <vector>

struct LightSpan {
    GLuint first;
    GLuint count;
};

struct LightCulling {
    agl::common::Interval<agl::Vec3> domain;
    std::array<GLint, 3> resolution = {0, 0, 0};

    std::vector<GLuint> light_indices;
    std::vector<LightSpan> light_spans;

    agl::opengl::Buffer light_index_ssbo;
    agl::opengl::Buffer light_span_ssbo;
};

inline
auto light_culling(
    const LightGroup& lg,
    agl::common::Interval<agl::Vec3> domain,
    std::array<GLint, 3> resolution)
{
    auto random = std::default_random_engine(agl::standard::random_seed());
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
        for(GLint i0 = 0; i0 < lc.resolution[0]; ++i0)
        for(GLint i1 = 0; i1 < lc.resolution[1]; ++i1)
        for(GLint i2 = 0; i2 < lc.resolution[2]; ++i2) {
            auto cell = agl::common::interval(
                agl::vec3(float(i0), float(i1), float(i2)) * step + lower_bound(lc.domain),
                agl::vec3(float(i0 + 1), float(i1 + 1), float(i2 + 1)) * step + lower_bound(lc.domain));
            auto first = GLuint(size(lc.light_indices));
            auto count = GLuint(0);
            for(GLuint li = 0; li < size(lg.light_properties); ++li) {
                auto& l = lg.light_properties[li];
                auto lp = l.position.xyz();
                auto d = agl::distance(clamp(cell, lp), lp);
                auto monomial_basis = agl::vec3(1.f, d, d * d);
                if(1.f / dot(monomial_basis, l.attenuation.xyz()) > 0.03f) {
                    lc.light_indices.push_back(li);
                    count += 1;
                }
            }
            auto& s = lc.light_spans.emplace_back(first, count);
            std::shuffle(
                data(lc.light_indices) + s.first,
                data(lc.light_indices) + s.first + s.count,
                random);
        }
    }
    {
        auto m = GLuint(0);
        for(auto& ls : lc.light_spans) {
            m = std::max(m, ls.count);
        }
        std::cout << "  light index count = " << size(lc.light_indices) << std::endl;
        std::cout << "  max light per cell = " << m << std::endl;
    }
    {
        if(size(lc.light_indices) > 0) {
            gl::NamedBufferStorage(
                lc.light_index_ssbo,
                std::span(lc.light_indices));
        }
        if(size(lc.light_spans) > 0) {
            gl::NamedBufferStorage(
                lc.light_span_ssbo,
                std::span(lc.light_spans));
        }
    }
    return lc;
}
