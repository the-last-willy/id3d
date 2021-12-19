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
