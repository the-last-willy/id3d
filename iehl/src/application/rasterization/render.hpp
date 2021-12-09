#pragma once

// #include "frustum_culling/all.hpp"
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

        auto draw_parameters = std::vector<DrawElementsParameters>();
        auto objects_bounds = std::vector<agl::common::Interval<agl::Vec3>>();
        auto primitive_offsets = std::vector<GLuint>();
        {
            for(auto& c : scene_grid.cells) {
                if(not is_empty(c)) {
                    draw_parameters.push_back(DrawElementsParameters{
                        .count = GLuint(3 * (c.last - c.first)),
                        .primitive_count = 1,
                        .offset = GLuint(3 * c.first),
                        .base_vertex = 0,
                        .base_instance = GLuint(size(draw_parameters))});
                    objects_bounds.push_back(c.bounds);
                    primitive_offsets.push_back(GLuint(c.first));
                }
            }
        }

        if(settings.rasterization.frustum_culling.mode == FrustumCullingMode::cpu) {
            auto frustum_clip_bounds = agl::common::interval(agl::vec3(-1.f), agl::vec3(1.f));
            auto frustum_world_bounds = agl::common::Interval<agl::Vec3>();
            { // Compute frustun worl bounds.
                auto cs = corners(frustum_clip_bounds);
                for(auto& c : cs) {
                    auto homogeneous = ctw * agl::vec4(c, 1.f);
                    c = homogeneous.xyz() / homogeneous[3];
                }
                frustum_world_bounds = agl::common::interval(cs[0]);
                for(std::size_t i = 1; i < 8; ++i) {
                    extend(frustum_world_bounds, cs[i]);
                }
            }

            auto accepted_draw_parameters = agl::standard::reserved_vector<DrawElementsParameters>(size(draw_parameters));
            for(const auto& dps : draw_parameters) {
                auto& object_bounds = objects_bounds[dps.base_instance];
                { // World space.
                    auto are_separated = false
                        or lower_bound(object_bounds)[0] > upper_bound(frustum_world_bounds)[0]
                        or lower_bound(object_bounds)[1] > upper_bound(frustum_world_bounds)[1]
                        or lower_bound(object_bounds)[2] > upper_bound(frustum_world_bounds)[2]
                        or upper_bound(object_bounds)[0] < lower_bound(frustum_world_bounds)[0]
                        or upper_bound(object_bounds)[1] < lower_bound(frustum_world_bounds)[1]
                        or upper_bound(object_bounds)[2] < lower_bound(frustum_world_bounds)[2];
                    if(are_separated) {
                        continue;
                    }
                }
                { // Clip space.

                }
                accepted_draw_parameters.push_back(dps);
            }
            // std::cout << size(accepted_draw_parameters);
            draw_parameters = std::move(accepted_draw_parameters);
        } else if(settings.rasterization.frustum_culling.mode == FrustumCullingMode::gpu) {
            auto frustum_clip_bounds = agl::common::interval(agl::vec3(-1.f), agl::vec3(1.f));
            auto frustum_world_bounds = agl::common::Interval<agl::Vec3>();
            { // Compute frustun worl bounds.
                auto cs = corners(frustum_clip_bounds);
                for(auto& c : cs) {
                    auto homogeneous = ctw * agl::vec4(c, 1.f);
                    c = homogeneous.xyz() / homogeneous[3];
                }
                frustum_world_bounds = agl::common::interval(cs[0]);
                for(std::size_t i = 1; i < 8; ++i) {
                    extend(frustum_world_bounds, cs[i]);
                }
            }

            auto objects_bounds_ssbo = agl::create(agl::buffer_tag);
            {
                auto objects_bounds4 = std::vector<agl::common::Interval<agl::Vec4>>(size(objects_bounds));
                for(std::size_t i = 0; i < size(objects_bounds); ++i) {
                    objects_bounds4[i] = agl::common::interval(
                        agl::vec4(lower_bound(objects_bounds[i]), 1.f),
                        agl::vec4(upper_bound(objects_bounds[i]), 1.f));
                }
                storage(objects_bounds_ssbo, std::span(objects_bounds4));
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, objects_bounds_ssbo);
            }
            
            auto input_parameters_ssbo = agl::create(agl::buffer_tag);
            storage(input_parameters_ssbo, std::span(draw_parameters));
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, input_parameters_ssbo);

            auto zero = std::array<GLuint, 1>{0};
            auto output_count_ssbo = agl::create(agl::buffer_tag);
            storage(output_count_ssbo, std::span(zero));
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, output_count_ssbo);

            auto output_parameters_ssbo = agl::create(agl::buffer_tag);
            storage(output_parameters_ssbo, std::span(draw_parameters));
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, output_parameters_ssbo);

            use(frustum_culling_shader.program);

            uniform(
                frustum_culling_shader.program,
                *agl::uniform_location(
                    frustum_culling_shader.program,
                    "frustum_world_bounds_lb"),
                agl::vec4(lower_bound(frustum_world_bounds), 1.f));
            uniform(
                frustum_culling_shader.program,
                *agl::uniform_location(
                    frustum_culling_shader.program,
                    "frustum_world_bounds_ub"),
                agl::vec4(upper_bound(frustum_world_bounds), 1.f));

            glDispatchCompute((size(objects_bounds) + 255) / 256, 1, 1);

            glMemoryBarrier(GL_ALL_BARRIER_BITS);

            GLuint output_count;
            glGetNamedBufferSubData(
                output_count_ssbo,
                0,
                4,
                &output_count);

            auto accepted_draw_parameters 
            = std::vector<DrawElementsParameters>(output_count);
            glGetNamedBufferSubData(
                output_parameters_ssbo,
                0,
                output_count * sizeof(DrawElementsParameters),
                data(accepted_draw_parameters));

            draw_parameters = std::move(accepted_draw_parameters);

            delete_(objects_bounds_ssbo);
            delete_(input_parameters_ssbo);
            delete_(output_count_ssbo);
            delete_(output_parameters_ssbo);
        }
        if(not empty(draw_parameters)) {
            auto primitive_offsets_ssbo = agl::create(agl::buffer_tag);

            bind(scene.program);

            {
                storage(primitive_offsets_ssbo, std::span(primitive_offsets));
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

            ::render(scene, draw_parameters);

            unbind(scene.program);

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
