#pragma once

#include "application.hpp"

void App::init() {
    { // Shader compiler.
        shader_compiler.log_folder = "logs/";
    }
    { // Render passes.
        assign_program(mesh_pass,
            shader::flat_shading(shader_compiler));
        mesh_pass.program->capabilities.emplace_back(
            agl::Capability::blend,
            []() {
                glBlendEquation(GL_FUNC_ADD);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            });
        mesh_pass.program->capabilities.emplace_back(
            agl::Capability::cull_face,
            []() {
                glCullFace(GL_BACK);
            });
        assign_program(wireframe_pass,
            shader::wireframe(shader_compiler));
    }
    { // Camera.
        if(auto pp = std::get_if<eng::PerspectiveProjection>(&camera.projection)) {
            pp->aspect_ratio = 16.f / 9.f;
        }
    }
}
