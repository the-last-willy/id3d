#pragma once

#include "render_ui.hpp"

#include <agl/engine/all.hpp>

void Application::render() {
    clear(agl::default_framebuffer, agl::depth_tag, 1.f);

    clear(wireframe_pass);

    auto nt = agl::engine::normal_transform(camera);
    auto wtc = agl::engine::world_to_clip(camera);

    if constexpr(false) {   
        bb_mesh = agl::standard::shared(
            agl::engine::instance(
                agl::engine::wireframe(
                    gizmo::box_wireframe())));
        auto bb = bounding_box(scene);
        subscribe(wireframe_pass, bb_mesh);
        bb_mesh->uniforms["color"]
        = std::make_shared<eng::Uniform<agl::Vec4>>(
            agl::vec4(0.f, 1.f, 0.f, 1.f));
        bb_mesh->uniforms["model_to_clip"]
        = std::make_shared<eng::Uniform<agl::Mat4>>(
            wtc * gizmo::box_wireframe_model_to_world(bb));
    }
    if(settings.rasterization_enabled) {
        uniform(scene.program, "world_to_clip", wtc);
        uniform(scene.program, "world_to_normal", nt);

        auto counts = std::vector<GLsizei>();
        auto offsets = std::vector<GLintptr>();
        for(auto& c : scene_grid.cells) {
            if(not is_empty(c)) {
                counts.push_back(GLsizei(3 * (c.last - c.first)));
                offsets.push_back(GLintptr(12 * c.first));
                // std::cout << c.first << " " << c.last - c.first << std::endl;
            }
        }

        // ::render(scene, counts, offsets);
        ::render(scene);
    }
    if(settings.bvh_debugging_enabled) {
        traverse(scene_bvh, [&, this](const BvhNode& bn, std::size_t depth) {
            auto has_children = bool(bn.inf_node) or bool(bn.sup_node);
            if(depth == settings.bvh_debugging_level
                or (depth < settings.bvh_debugging_level and not has_children))
            {
                auto&& instance = subscribe(wireframe_pass, box_wireframe);
                instance->uniforms["color"]
                = std::make_shared<eng::Uniform<agl::Vec4>>(
                    agl::vec4(0.f, 1.f, 0.f, 1.f));
                instance->uniforms["model_to_clip"]
                = std::make_shared<eng::Uniform<agl::Mat4>>(
                    wtc * gizmo::box_wireframe_model_to_world(bn.bounds));
            }
        });
    }
    if(settings.object_grid_debugging_enabled) {
        for(auto& cell : scene_grid.cells) {
            if(not is_empty(cell)) {
                auto&& instance = subscribe(wireframe_pass, box_wireframe);
                instance->uniforms["color"]
                = std::make_shared<eng::Uniform<agl::Vec4>>(
                    agl::vec4(0.f, 1.f, 0.f, 1.f));
                instance->uniforms["model_to_clip"]
                = std::make_shared<eng::Uniform<agl::Mat4>>(
                    wtc * gizmo::box_wireframe_model_to_world(cell.bounds));
            }
        }
    }
    {
        agl::engine::render(wireframe_pass);
    }
    {
        render_ui(*this);
    }
}
