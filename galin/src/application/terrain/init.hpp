#pragma once

#include "application.hpp"
#include "reset_terrain.hpp"
#include "update_terrain.hpp"

#include <cmath>

void App::init() {
    { // Shader compiler.
        shader_compiler.log_folder = "logs/";
    }
    { // Render passes.
        assign_program(mesh_pass,
            shader::flat_shading(shader_compiler));
    }
    { // Terrain.
        reset_terrain(*this);
    }
    { // Forest.
        auto trees = std::vector<Tree>();
        for(int i = 0; i < 10; ++i) {
            auto color = (i % 2 == 0) ? agl::vec4(1.f, 0.f, 0.f, 1.f) : agl::vec4(0.f, 1.f, 0.f, 1.f);
            for(int j = 0; j < 10; ++j) {
                trees.emplace_back(Tree{
                    .color = color,
                    .position = {float(i), float(j), 0}});
            }
        }
        
        forest = ::forest(ForestParameters{
            .shader_compiler = &shader_compiler,
            .trees = &trees});
    }
    { // Camera.
        if(auto pp = std::get_if<eng::PerspectiveProjection>(&camera.projection)) {
            pp->aspect_ratio = 16.f / 9.f;
        }
        { // Center camera above terrain.
            auto d = terrain.settings.domain;
            auto d_center = lower_bound(d) + length(d) / 2.f;
            camera.view.position = agl::vec3(d_center[0], 20.f, d_center[1]);
            camera.view.pitch = -agl::constant::pi / 2.f;
        }
    }
}
