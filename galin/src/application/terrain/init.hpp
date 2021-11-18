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
        ts.domain = agl::common::interval(agl::vec2(0.f), agl::vec2(1.f));
        ts.resolution = {100, 100};
        terrain = create(ts);
        for(std::size_t i = 0; i < resolution(terrain)[0]; ++i)
        for(std::size_t j = 0; j < resolution(terrain)[1]; ++j) {
            at(terrain.heights, i, j) = 10 * std::cos(float(i) / 10.f) / 100.f;
        }
        /////////////////////////////////////////////////////////////////////
        compute_data(terrain);
        for(int i = 0; i < 2; ++i) {
            // steepest_compute(terrain);
            // stream_power_erosion(terrain);
            //hillslope_erosion(terrain);
        }
        for(std::size_t i = 0; i < resolution(terrain)[0]; ++i)
        for(std::size_t j = 0; j < resolution(terrain)[1]; ++j) {
            at(terrain.colors, i, j) = agl::vec3(1 / (at(terrain.laplaciens, i, j) / 100 + 1));
        }
        /////////////////////////////////////////////////////////////////////
        update_cpu(terrain);
        update_gpu(terrain);
    }
}
