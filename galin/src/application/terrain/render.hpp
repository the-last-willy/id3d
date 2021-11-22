#pragma once

#include "application.hpp"
#include "ui.hpp"

void App::render() {
    clear(agl::default_framebuffer, agl::depth_tag, 1.f);

    clear(mesh_pass);

    auto wtc = agl::engine::world_to_clip(camera);
    auto wte = agl::engine::world_to_eye(camera);

    terrain.gpu_mesh->uniforms["model_to_clip"]
    = std::make_shared<eng::Uniform<agl::Mat4>>(wtc);
    terrain.gpu_mesh->uniforms["model_to_eye"]
    = std::make_shared<eng::Uniform<agl::Mat4>>(wte);
    subscribe(mesh_pass, terrain.gpu_mesh);

    agl::engine::render(mesh_pass);

    ui();
}
