#pragma once

#include "application.hpp"

inline
void generate_trees(App& app) {
    auto trees = std::vector<Tree>();
    {
        auto d = domain(app.terrain);
        auto r = resolution(app.terrain);
        auto rand_01 = std::uniform_real_distribution<float>(0.f, 1.f);
        auto r2d = mapping(
            agl::common::interval(
                agl::vec2(0.f),
                agl::vec2(float(r[0] - 1), float(r[1] - 1))),
            agl::common::interval(
                agl::vec2(lower_bound(d)[0], lower_bound(d)[1]),
                agl::vec2(upper_bound(d)[0], upper_bound(d)[1])));
        for(std::size_t i0 = 0; i0 < r[0]; ++i0)
        for(std::size_t i1 = 0; i1 < r[1]; ++i1) {
            auto xz = r2d(agl::vec2(float(i0), float(i1)))
            + agl::vec2(rand_01(app.random_generator) - 0.5f, rand_01(app.random_generator) - 0.5f) / 50.f;
            auto y = at(app.terrain.height.values, i0, i1);
            auto color = agl::vec4(0.f, 0.f, 0.f, 1.f);
            auto scale = 0.f;
            auto p = rand_01(app.random_generator);
            auto vpv = at(app.terrain.vegetation_probability.values, i0, i1);
            if(p < vpv[0]) {
                color = agl::vec4(1.f, 0.f, 0.f, 1.f);
                scale = vpv[0];
            } else {
                p -= vpv[0];
                if(p < vpv[1]) {
                    color = agl::vec4(0.f, 1.f, 0.f, 1.f);
                    scale = vpv[1];
                } else {
                    p -= vpv[1];
                    if(p < vpv[2]) {
                        color = agl::vec4(0.f, 0.f, 1.f, 1.f);
                        scale = vpv[2];
                    } else {
                        continue;
                    }
                }
            }
            trees.emplace_back(Tree{
                .color = color,
                .position = agl::vec3(xz[0], y, xz[1]),
                .scale = 0.01f});
        }
    }
    app.forest = forest(ForestParameters{
        .shader_compiler = &app.shader_compiler,
        .trees = &trees});
}
