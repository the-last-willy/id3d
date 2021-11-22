#pragma once

#include "application.hpp"

#include <cmath>

void App::init() {
    { // Shader compiler.
        shader_compiler.log_folder = "logs/";
    }
    { // Render passes.
        assign_program(mesh_pass,
            shader::flat_shading(shader_compiler));
    }
    { // Camera.
        if(auto pp = std::get_if<eng::PerspectiveProjection>(&camera.projection)) {
            pp->aspect_ratio = 16.f / 9.f;
        }
    }
    { // Terrain.
        auto ts = TerrainSettings();
        ts.domain = agl::common::interval(agl::vec2(0.f), agl::vec2(100.f));
        ts.resolution = {500, 500};
        terrain = create(ts);
        auto mapping = index_to_world_mapping(terrain);
        for(std::size_t i = 0; i < resolution(terrain)[0]; ++i)
        for(std::size_t j = 0; j < resolution(terrain)[1]; ++j) {
            auto pos = mapping(agl::vec2(i, j));
            at(terrain.heights, i, j) 
            = std::cos(float(i) / 10.f);
            // = gradient_noise(agl::vec2(i / 20.f, j / 20.f)) * 5.f;
        //     + abs(gradient_noise(agl::vec2(i / 10.f, j / 10.f)) * 10.f)
        //     + abs(gradient_noise(agl::vec2(i / 2.f * 4.f, j / 20.f * 4.f)) / 10.f / 16.f)
        //     + gradient_noise(agl::vec2(i / 20.f * 8.f, j / 20.f * 8.f)) / 10.f / 64.f;
        }
        update_all(terrain);
    }
}
