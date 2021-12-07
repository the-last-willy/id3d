#pragma once

#include "frustum_culling/all.hpp"
#include "render_ui.hpp"

#include <agl/engine/all.hpp>

void Application::render() {
    clear(agl::default_framebuffer, agl::depth_tag, 1.f);

    clear(wireframe_pass);

    auto ctw = agl::engine::clip_to_world(camera);
    auto nt = agl::engine::normal_transform(camera);
    auto wtc = agl::engine::world_to_clip(camera);
    auto wtv = agl::engine::world_to_eye(camera);

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
        uniform(scene.program, "world_to_view", wtv);

        auto frustum_aabb = agl::common::interval(
            agl::vec3(-1.f), agl::vec3(1.f));

        auto counts = std::vector<GLsizei>();
        auto offsets4 = std::vector<GLuint>();
        auto offsets = std::vector<GLintptr>();

        // auto draw_parameters = agl::Buffer();
        // storage<DrawElementsParameters>(
        //     draw_parameters,
        //     scene_grid.non_empty_cell_count,
        //     GL_MAP_WRITE_BIT !);
        // auto draw_parameters_count = GLsizei(0);

        for(auto& c : scene_grid.cells) {
            if(not is_empty(c)) {
                // if(aabb_intersecting(c.bounds, wtc, frustum_aabb, ctw)) {
                    counts.push_back(GLsizei(3 * (c.last - c.first)));
                    offsets4.push_back(GLuint(c.first));
                    offsets.push_back(GLintptr(12 * c.first));
                    // auto& dps = draw_parameters.emplace_back();
                    // dps.count = GLuint(3 * (c.last - c.first));
                    // dps.offset = GLuint(12 * c.first);
                // }
            }
        }

        if(not empty(counts)) {
            auto primitive_offsets_ssbo = agl::create(agl::buffer_tag);
            {
                storage(primitive_offsets_ssbo, std::span(offsets4));
                glBindBufferBase(
                    GL_SHADER_STORAGE_BUFFER, 2, primitive_offsets_ssbo);
            }
            {
                glBindBufferBase(
                    GL_SHADER_STORAGE_BUFFER, 4,
                    scene_grid_lights.light_index_ssbo);
                glBindBufferBase(
                    GL_SHADER_STORAGE_BUFFER, 5,
                    scene_grid_lights.light_span_ssbo);
            }

            update_lights(scene, wtv);

            // ::render(scene, draw_parameters);
            ::render(scene, counts, offsets);
            // ::render(scene);

            delete_(primitive_offsets_ssbo);
        }
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
