#pragma once

#include "application.hpp"
#include "ui.hpp"

void App::render() {
    clear(agl::default_framebuffer, agl::depth_tag, 1.f);

    clear(mesh_pass);
    clear(wireframe_pass);

    auto wtc = agl::engine::world_to_clip(camera);
    auto wte = agl::engine::world_to_eye(camera);

    if(settings.show_triangulation) {
        for(auto& p : object.patches) {
            p.gpu_tesselation->uniforms["model_to_clip"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(wtc);
            p.gpu_tesselation->uniforms["model_to_eye"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(wte);
            subscribe(mesh_pass, p.gpu_tesselation);
        }
    }
    if(settings.show_control_points) {
        for(auto& p : object.patches) {
            p.gpu_control_mesh->uniforms["model_to_clip"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(wtc);
            subscribe(wireframe_pass, p.gpu_control_mesh);
        }
    }
    if(settings.show_revolution) {
        object.revolution_surface.gpu_tesselation->uniforms["model_to_clip"]
        = std::make_shared<eng::Uniform<agl::Mat4>>(wtc);
        subscribe(mesh_pass, object.revolution_surface.gpu_tesselation);
    }
    if(settings.show_rev_control_curve) {
        object.revolution_surface.gpu_control_curve->uniforms["model_to_clip"]
        = std::make_shared<eng::Uniform<agl::Mat4>>(wtc);
        subscribe(wireframe_pass, object.revolution_surface.gpu_control_curve);
    }
    if(settings.show_gizmo) {
        auto model_to_world = agl::translation(settings.transform_position)
        * agl::scaling3(settings.transform_radius);
        transform_aoe->uniforms["color_factor"]
        = std::make_shared<eng::Uniform<agl::Vec4>>(agl::vec4(1.f, 1.f, 0.f, 0.5f));
        transform_aoe->uniforms["model_to_clip"]
        = std::make_shared<eng::Uniform<agl::Mat4>>(wtc * model_to_world);
        subscribe(mesh_pass, transform_aoe);
    }
    if(settings.show_bounding_box) {
        object.gpu_bounding_box->uniforms["model_to_clip"]
        = std::make_shared<eng::Uniform<agl::Mat4>>(wtc
            * gizmo::box_wireframe_model_to_world(object.bounding_box));
        subscribe(wireframe_pass, object.gpu_bounding_box);
    }
    { // FFD.
        if(settings.ffd_show_control_grid) {
            if(ffd_control_mesh) {
                ffd_control_mesh->uniforms["model_to_clip"]
                = std::make_shared<eng::Uniform<agl::Mat4>>(wtc);
                subscribe(wireframe_pass, ffd_control_mesh);
            }
        }
    }
    agl::engine::render(mesh_pass);
    agl::engine::render(wireframe_pass);

    ui();
}
