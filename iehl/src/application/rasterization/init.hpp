#pragma once

#include "scene/all.hpp"
#include "shader/all.hpp"

#include <id3d/common/all.hpp>

void Application::init() {
    {
        shader_compiler.log_folder = "logs/";
    }
    {
        wireframe_pass.program = agl::standard::shared(
            shader::wireframe(shader_compiler));
        bb_mesh = agl::standard::shared(
            agl::engine::instance(
                eng::Mesh()));
    }
    {
        // scene = wavefront_scene("C:/Users/Willy/Desktop/data/bistro-small/exterior.obj");
        scene = wavefront_scene("C:/Users/Willy/Desktop/data/wavefront/CornellBox/cornell-box.obj");
        scene.program = render_program(shader_compiler);
        scene.program.capabilities.emplace_back(
            agl::Capability::depth_test, 
            []() { glDepthFunc(GL_LEQUAL); });
        initialize_gpu(scene);
    }
    { // Camera.
        if(auto pp = std::get_if<eng::PerspectiveProjection>(&camera.projection)) {
            pp->aspect_ratio = 16.f / 9.f;
            pp->z_far = 100.f;
        }
    }
}
