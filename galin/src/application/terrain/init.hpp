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
        ts.resolution = {500, 500};
        terrain = create(ts);
        for(std::size_t i = 0; i < resolution(terrain)[0]; ++i)
        for(std::size_t j = 0; j < resolution(terrain)[1]; ++j) {
            at(terrain.heights, i, j) = std::cos(float(i) / 10.f) / 100.f;
        }
        /////////////////////////////////////////////////////////////////////
        auto nx = terrain.settings.resolution[0];
        auto ny = terrain.settings.resolution[1];
        terrain.gradients = agl::common::grid<agl::Vec2>(nx, ny);
        auto dx = 1;
        auto dy = 1;
        for(std::size_t i = 1; i < nx - 2; ++i) {
            for(std::size_t j = 1; j < ny; ++j) {
                auto dfdx = (at(terrain.heights, i + 1, j) - at(terrain.heights, i - 1, j)) / (2 * dx);
                auto dfdy = (at(terrain.heights, i, j + 1) - at(terrain.heights, i, j - 1)) / (2 * dy);
                at(terrain.gradients, i, j) = agl::vec2(dfdx, dfdy);
            }
        }
        for(size_t j = 1; j < ny - 2; ++j) {
            //replace 0 by domain's lower bound
            auto left_dfdx = (at(terrain.heights, 1, j) - at(terrain.heights, 0, j)) / dx;
            auto right_dfdx = (at(terrain.heights, nx, j) - at(terrain.heights, nx - 1, j)) / dx;
            auto left_dfdy = (at(terrain.heights, 0, j + 1) - at(terrain.heights, 0, j - 1)) / (2 * dy);
            auto right_dfdy = (at(terrain.heights, nx, j + 1) - at(terrain.heights, nx, j - 1)) / (2 * dy);
            at(terrain.gradients, 0, j) = agl::vec2(left_dfdx, left_dfdy);
            at(terrain.gradients, nx - 1, j) = agl::vec2(right_dfdx, left_dfdy);
        }
        for(size_t i = 1; i < nx - 2; ++i) {
            //replace 0 by domain's lower bound
            auto up_dfdx = (at(terrain.heights, i + 1, ny) - at(terrain.heights, i - 1, ny)) / (2 * dx);
            auto down_dfdx = (at(terrain.heights, i + 1, 0) - at(terrain.heights, i - 1, 0)) / (2 * dx);
            auto up_dfdy = (at(terrain.heights, 1, ny) - at(terrain.heights, 0, ny - 1)) / dy;
            auto down_dfdy = (at(terrain.heights, i, 1) - at(terrain.heights, i, 0)) / dy;
            at(terrain.gradients, i, ny) = agl::vec2(up_dfdx, up_dfdy);
            at(terrain.gradients, i, 0) = agl::vec2(down_dfdx, down_dfdy);
        }
        // corner
        at(terrain.gradients, 0, 0) = agl::vec2(
                                            (at(terrain.heights, 1, 0) - at(terrain.heights, 0, 0)) / dx,
                                            (at(terrain.heights, 0, 1) - at(terrain.heights, 0, 0)) / dy
                                        );
        at(terrain.gradients, nx, 0) = agl::vec2(
                                            (at(terrain.heights, nx, 0) - at(terrain.heights, nx - 1, 0)) / dx,
                                            (at(terrain.heights, nx, 1) - at(terrain.heights, nx, 0)) / dy
                                        );
        at(terrain.gradients, 0, ny) = agl::vec2(
                                            (at(terrain.heights, 1, ny) - at(terrain.heights, 0, ny)) / dx,
                                            (at(terrain.heights, 0, ny) - at(terrain.heights, 0, ny - 1)) / dy
                                        );
        at(terrain.gradients, nx - 1, ny - 1) = agl::vec2(
                                            (at(terrain.heights, nx, ny) - at(terrain.heights, nx - 1, ny)) / dx,
                                            (at(terrain.heights, nx, ny) - at(terrain.heights, nx, ny - 1)) / dy
                                        );

        terrain.normals = agl::common::grid<agl::Vec3>(nx, ny);
        terrain.slopes = agl::common::grid<float>(nx, ny);
        for(std::size_t i = 0; i < nx; ++i) {
            for(std::size_t j = 0; j < ny; ++j) {
                at(terrain.normals, i, j) = agl::vec3(-at(terrain.gradients, i, j), 1);
                at(terrain.slopes, i, j) = agl::length(at(terrain.gradients, i, j));
            }
        }
        /////////////////////////////////////////////////////////////////////
        update_cpu(terrain);
        update_gpu(terrain);
    }
}
