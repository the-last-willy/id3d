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
        terrain = load_srtm1(
            "./galin/data/srtm1/N45E005.hgt",
            {1000, 500},
            {500, 500});
    }
}
