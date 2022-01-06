#pragma once

// #include "frustum_culling/all.hpp"
#include "render_ui.hpp"

#include <agl/engine/all.hpp>

void Application::render() {

    auto m2c = agl::engine::world_to_clip(camera);
    auto m2w = agl::mat4(agl::identity);

    auto eye_world_position = camera.view.position;

    bind(hdr_framebuffer);
    // gl::ClearNamedFramebuffer(0, GL_DEPTH, 0, 1.f);
    gl::ClearNamedFramebuffer(hdr_framebuffer.fbo, GL_DEPTH, 0, 1.f);
    auto clear_color = std::array{0.f, 0.f, 0.f};
    glClearNamedFramebufferfv(hdr_framebuffer.fbo, GL_COLOR, 0, data(clear_color));

    glUseProgram(forward_renderer.program);

    upload(forward_renderer, light_culling);
    upload(forward_renderer, scene.light_group);
    upload(forward_renderer, scene.material_group);
    upload(forward_renderer, scene.objects.data);
    upload(forward_renderer, scene.objects.topology);

    glViewport(0, 0, 1280, 720);

    glProgramUniform3fv(forward_renderer.program,
        forward_renderer.eye_position_location_location,
        1, data(eye_world_position));
    glProgramUniformMatrix4fv(forward_renderer.program,
        forward_renderer.model_to_clip_location,
        1, GL_FALSE, data(m2c));
    glProgramUniformMatrix4fv(forward_renderer.program,
        forward_renderer.model_to_world_location,
        1, GL_FALSE, data(m2w));
    glProgramUniformMatrix4fv(forward_renderer.program,
        forward_renderer.model_to_world_normal_location,
        1, GL_FALSE, data(m2w));

    glBindVertexArray(forward_rendering_vao);

    glVertexArrayElementBuffer(forward_rendering_vao, 
        scene.objects.topology.element_buffer);

    // glCullFace(GL_BACK);
    // glEnable(GL_CULL_FACE);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, scene.objects.topology.draw_command_buffer);

    glMultiDrawElementsIndirect(
        GL_TRIANGLES, GL_UNSIGNED_INT,
        0, scene.objects.topology.draw_count, 0);

    // glDisable(GL_CULL_FACE);

    glDisable(GL_DEPTH_TEST);

    { // Occlusion culling.
        bind_for_drawing(occlusion_culler);

        gl::ClearNamedFramebuffer(occlusion_culler.depth_fbo,
            GL_DEPTH, 0, 1.f);

        glViewport(0, 0, 512, 256);

        glProgramUniformMatrix4fv(occlusion_culler.draw_program,
            occlusion_culler.draw_model_to_clip_uniform_location,
            1, GL_FALSE, data(m2c));

        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);

        glMultiDrawElementsIndirect(
            GL_TRIANGLES, GL_UNSIGNED_INT,
            0, scene.objects.topology.draw_count, 0);

        glDisable(GL_DEPTH_TEST);

        generate_mipmap(occlusion_culler);

        occlusion_culler.draw_count = scene.objects.topology.draw_count;
        occlusion_culler.draw_indirect_commands = scene.objects.topology.draw_commands;
        auto filtered_commands = std::vector<gl::DrawElementsIndirectCommand>();

        auto& object_bounds = scene.objects.data.object_bounds;
        for(GLsizei i = 0; i < occlusion_culler.draw_count; ++i) {
            auto& cmd = occlusion_culler.draw_indirect_commands[i];
            auto bounds = object_bounds[cmd.baseInstance];
            auto corners = std::array{
                agl::vec4(lower_bound(bounds)[0], lower_bound(bounds)[1], lower_bound(bounds)[2], 1.f),
                agl::vec4(lower_bound(bounds)[0], lower_bound(bounds)[1], upper_bound(bounds)[2], 1.f),
                agl::vec4(lower_bound(bounds)[0], upper_bound(bounds)[1], lower_bound(bounds)[2], 1.f),
                agl::vec4(lower_bound(bounds)[0], upper_bound(bounds)[1], upper_bound(bounds)[2], 1.f),
                agl::vec4(upper_bound(bounds)[0], lower_bound(bounds)[1], lower_bound(bounds)[2], 1.f),
                agl::vec4(upper_bound(bounds)[0], lower_bound(bounds)[1], upper_bound(bounds)[2], 1.f),
                agl::vec4(upper_bound(bounds)[0], upper_bound(bounds)[1], lower_bound(bounds)[2], 1.f),
                agl::vec4(upper_bound(bounds)[0], upper_bound(bounds)[1], upper_bound(bounds)[2], 1.f),
            };
            auto c = m2c * corners[0];
            c /= c[3];
            auto clip_bounds = agl::common::interval(corners[0]);
            auto clip_box = agl::common::interval(agl::vec3(-1.f), agl::vec3(0.999f));
            for(auto corner : corners) {
                c = m2c * corner;
                c /= c[3];
                extend(clip_bounds, clamp(clip_box, c));
            }

            auto l = length(clip_bounds);
            auto level = GLuint(std::ceil(std::log2(std::max(l[0], l[1]))));

            auto depth_image = occlusion_culler.depth_images.at(level);

            auto w = occlusion_culler.depth_image_widths[level];
            auto h = occlusion_culler.depth_image_heights[level];

            auto x0 = GLint((lower_bound(clip_bounds)[0] * .5 + .5) * w);
            auto y0 = GLint((lower_bound(clip_bounds)[1] * .5 + .5) * h);
            auto x1 = GLint((upper_bound(clip_bounds)[0] * .5 + .5) * w);
            auto y1 = GLint((upper_bound(clip_bounds)[1] * .5 + .5) * h);

            auto d00 = depth_image[x0 + y0 * w];
            auto d01 = depth_image[x0 + y1 * w];
            auto d10 = depth_image[x1 + y0 * w];
            auto d11 = depth_image[x1 + y1 * w];

            auto max = std::max(std::max(d00, d01), std::max(d10, d11));

            if(lower_bound(bounds)[2] <= max) {
                std::cout << "  " << i << " accepted" << std::endl;
                filtered_commands.emplace_back(cmd);
            } else {
                std::cout << "  " << i << " rejected" << std::endl;
            }
        }

        std::cout << "OC " << size(occlusion_culler.draw_indirect_commands) << " " << size(filtered_commands) << std::endl;
        occlusion_culler.draw_indirect_commands = std::move(filtered_commands);
    }
    { // Tone mapping.
        bind(tone_mapper);
        bind(tone_mapper, hdr_framebuffer);

        glViewport(0, 0, 1280, 720);

        draw(tone_mapper);
    }

    // auto ctw = agl::engine::clip_to_world(camera);
    // auto nt = agl::engine::normal_transform(camera);
    // auto wtc = agl::engine::world_to_clip(camera);
    // auto wtv = agl::engine::world_to_eye(camera);

    // if constexpr(false) {   
    //     bb_mesh = agl::standard::shared(
    //         agl::engine::instance(
    //             agl::engine::wireframe(
    //                 gizmo::box_wireframe())));
    //     auto bb = bounding_box(scene);
    //     subscribe(wireframe_pass, bb_mesh);
    //     bb_mesh->uniforms["color"]
    //     = std::make_shared<eng::Uniform<agl::Vec4>>(
    //         agl::vec4(0.f, 1.f, 0.f, 1.f));
    //     bb_mesh->uniforms["model_to_clip"]
    //     = std::make_shared<eng::Uniform<agl::Mat4>>(
    //         wtc * gizmo::box_wireframe_model_to_world(bb));
    // }
    // if(settings.rasterization_enabled) {
    //     // uniform(scene.program, "world_to_clip", wtc);
    //     // uniform(scene.program, "world_to_normal", nt);
    //     // uniform(scene.program, "world_to_view", wtv);

    //     auto draw_parameters = std::vector<DrawElementsParameters>();
    //     auto objects_bounds = std::vector<agl::common::Interval<agl::Vec3>>();
    //     auto primitive_offsets = std::vector<GLuint>();
    //     {
    //         for(auto& c : scene_grid.cells) {
    //             if(not is_empty(c)) {
    //                 draw_parameters.push_back(DrawElementsParameters{
    //                     .count = GLuint(3 * (c.last - c.first)),
    //                     .primitive_count = 1,
    //                     .offset = GLuint(3 * c.first),
    //                     .base_vertex = 0,
    //                     .base_instance = GLuint(size(draw_parameters))});
    //                 objects_bounds.push_back(c.bounds);
    //                 primitive_offsets.push_back(GLuint(c.first));
    //             }
    //         }
    //     }

    //     auto fc_ctw = agl::engine::clip_to_world(frustum_culling_camera);
    //     auto fc_wtc = agl::engine::world_to_clip(frustum_culling_camera);
    //     if(settings.rasterization.frustum_culling.mode == FrustumCullingMode::cpu) {
    //         auto frustum_clip_bounds = agl::common::interval(agl::vec3(-1.f), agl::vec3(1.f));
    //         auto frustum_world_bounds = agl::common::Interval<agl::Vec3>();
    //         { // Compute frustun worl bounds.
    //             auto cs = corners(frustum_clip_bounds);
    //             for(auto& c : cs) {
    //                 auto homogeneous = fc_ctw * agl::vec4(c, 1.f);
    //                 c = homogeneous.xyz() / homogeneous[3];
    //             }
    //             frustum_world_bounds = agl::common::interval(cs[0]);
    //             for(std::size_t i = 1; i < 8; ++i) {
    //                 extend(frustum_world_bounds, cs[i]);
    //             }
    //         }

    //         auto accepted_draw_parameters = agl::standard::reserved_vector<DrawElementsParameters>(size(draw_parameters));
    //         for(const auto& dps : draw_parameters) {
    //             auto& object_bounds = objects_bounds[dps.base_instance];
    //             { // World space.
    //                 auto are_separated = false
    //                     or lower_bound(object_bounds)[0] > upper_bound(frustum_world_bounds)[0]
    //                     or lower_bound(object_bounds)[1] > upper_bound(frustum_world_bounds)[1]
    //                     or lower_bound(object_bounds)[2] > upper_bound(frustum_world_bounds)[2]
    //                     or upper_bound(object_bounds)[0] < lower_bound(frustum_world_bounds)[0]
    //                     or upper_bound(object_bounds)[1] < lower_bound(frustum_world_bounds)[1]
    //                     or upper_bound(object_bounds)[2] < lower_bound(frustum_world_bounds)[2];
    //                 if(are_separated) {
    //                     continue;
    //                 }
    //             }
    //             { // Clip space.
    //                 auto object_clip_bounds = agl::common::Interval<agl::Vec3>();
    //                 {
    //                     auto cs = corners(object_bounds);
    //                     for(auto& c : cs) {
    //                         auto homogeneous = fc_wtc * agl::vec4(c, 1.f);
    //                         c = homogeneous.xyz() / homogeneous[3];
    //                     }
    //                     object_clip_bounds = agl::common::interval(cs[0]);
    //                     for(std::size_t i = 1; i < 8; ++i) {
    //                         extend(object_clip_bounds, cs[i]);
    //                     }
    //                 }
    //                 auto are_separated = false
    //                     or lower_bound(object_clip_bounds)[0] > upper_bound(frustum_clip_bounds)[0]
    //                     or lower_bound(object_clip_bounds)[1] > upper_bound(frustum_clip_bounds)[1]
    //                     or lower_bound(object_clip_bounds)[2] > upper_bound(frustum_clip_bounds)[2]
    //                     or upper_bound(object_clip_bounds)[0] < lower_bound(frustum_clip_bounds)[0]
    //                     or upper_bound(object_clip_bounds)[1] < lower_bound(frustum_clip_bounds)[1]
    //                     or upper_bound(object_clip_bounds)[2] < lower_bound(frustum_clip_bounds)[2];
    //                 if(are_separated) {
    //                     continue;
    //                 }
    //             }
    //             accepted_draw_parameters.push_back(dps);
    //         }

    //         statistics.frustum_culling.accepted_count
    //         = size(accepted_draw_parameters);
    //         statistics.frustum_culling.rejected_count
    //         = size(draw_parameters) - size(accepted_draw_parameters);

    //         draw_parameters = std::move(accepted_draw_parameters);
    //     } else if(settings.rasterization.frustum_culling.mode == FrustumCullingMode::gpu) {
    //         auto frustum_clip_bounds = agl::common::interval(agl::vec3(-1.f), agl::vec3(1.f));
    //         auto frustum_world_bounds = agl::common::Interval<agl::Vec3>();
    //         { // Compute frustun worl bounds.
    //             auto cs = corners(frustum_clip_bounds);
    //             for(auto& c : cs) {
    //                 auto homogeneous = fc_ctw * agl::vec4(c, 1.f);
    //                 c = homogeneous.xyz() / homogeneous[3];
    //             }
    //             frustum_world_bounds = agl::common::interval(cs[0]);
    //             for(std::size_t i = 1; i < 8; ++i) {
    //                 extend(frustum_world_bounds, cs[i]);
    //             }
    //         }

    //         auto objects_bounds_ssbo = agl::create(agl::buffer_tag);
    //         {
    //             auto objects_bounds4 = std::vector<agl::common::Interval<agl::Vec4>>(size(objects_bounds));
    //             for(std::size_t i = 0; i < size(objects_bounds); ++i) {
    //                 objects_bounds4[i] = agl::common::interval(
    //                     agl::vec4(lower_bound(objects_bounds[i]), 1.f),
    //                     agl::vec4(upper_bound(objects_bounds[i]), 1.f));
    //             }
    //             storage(objects_bounds_ssbo, std::span(objects_bounds4));
    //             glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, objects_bounds_ssbo);
    //         }
            
    //         auto input_parameters_ssbo = agl::create(agl::buffer_tag);
    //         storage(input_parameters_ssbo, std::span(draw_parameters));
    //         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, input_parameters_ssbo);

    //         auto zero = std::array<GLuint, 1>{0};
    //         auto output_count_ssbo = agl::create(agl::buffer_tag);
    //         storage(output_count_ssbo, std::span(zero));
    //         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, output_count_ssbo);

    //         auto output_parameters_ssbo = agl::create(agl::buffer_tag);
    //         storage(output_parameters_ssbo, std::span(draw_parameters));
    //         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, output_parameters_ssbo);

    //         // use(frustum_culling_shader.program);

    //         // uniform(
    //         //     frustum_culling_shader.program,
    //         //     *agl::uniform_location(
    //         //         frustum_culling_shader.program,
    //         //         "frustum_clip_bounds.lb"),
    //         //     agl::vec4(lower_bound(frustum_clip_bounds), 1.f));
    //         // uniform(
    //         //     frustum_culling_shader.program,
    //         //     *agl::uniform_location(
    //         //         frustum_culling_shader.program,
    //         //         "frustum_clip_bounds.ub"),
    //         //     agl::vec4(upper_bound(frustum_clip_bounds), 1.f));
    //         // uniform(
    //         //     frustum_culling_shader.program,
    //         //     *agl::uniform_location(
    //         //         frustum_culling_shader.program,
    //         //         "frustum_world_bounds.lb"),
    //         //     agl::vec4(lower_bound(frustum_world_bounds), 1.f));
    //         // uniform(
    //         //     frustum_culling_shader.program,
    //         //     *agl::uniform_location(
    //         //         frustum_culling_shader.program,
    //         //         "frustum_world_bounds.ub"),
    //         //     agl::vec4(upper_bound(frustum_world_bounds), 1.f));
    //         // uniform(
    //         //     frustum_culling_shader.program,
    //         //     *agl::uniform_location(
    //         //         frustum_culling_shader.program,
    //         //         "world_to_clip"),
    //         //     fc_wtc);

    //         glDispatchCompute((GLuint(size(objects_bounds)) + 255) / 256, 1, 1);

    //         glMemoryBarrier(GL_ALL_BARRIER_BITS);

    //         GLuint output_count;
    //         glGetNamedBufferSubData(
    //             output_count_ssbo,
    //             0,
    //             4,
    //             &output_count);

    //         auto accepted_draw_parameters 
    //         = std::vector<DrawElementsParameters>(output_count);
    //         glGetNamedBufferSubData(
    //             output_parameters_ssbo,
    //             0,
    //             output_count * sizeof(DrawElementsParameters),
    //             data(accepted_draw_parameters));

    //         statistics.frustum_culling.accepted_count
    //         = size(accepted_draw_parameters);
    //         statistics.frustum_culling.rejected_count
    //         = size(draw_parameters) - size(accepted_draw_parameters);

    //         draw_parameters = std::move(accepted_draw_parameters);

    //         delete_(objects_bounds_ssbo);
    //         delete_(input_parameters_ssbo);
    //         delete_(output_count_ssbo);
    //         delete_(output_parameters_ssbo);
    //     }
    //     if(not empty(draw_parameters)) {
    //         auto primitive_offsets_ssbo = agl::create(agl::buffer_tag);

    //         // bind(scene.program);

    //         {
    //             storage(primitive_offsets_ssbo, std::span(primitive_offsets));
    //             glBindBufferBase(
    //                 GL_SHADER_STORAGE_BUFFER, 2, primitive_offsets_ssbo);
    //         }
    //         { // Lights.

    //         }
    //         { // Light culling.
    //             // glUniform3fv(
    //             //     agl::uniform_location(scene.program.program, "light_culling_domain_lower_bound"),
    //             //     1, data(lower_bound(light_grid.domain)));
    //             // glUniform3fv(
    //             //     agl::uniform_location(scene.program.program, "light_culling_domain_upper_bound"),
    //             //     1, data(upper_bound(light_grid.domain)));
    //             // glUniform3fv(
    //             //     agl::uniform_location(scene.program.program, "light_culling_resolution"),
    //             //     1, data(std::array{float(light_grid.resolution[0]), float(light_grid.resolution[1]), float(light_grid.resolution[2])}));
    //             // glBindBufferBase(
    //             //     GL_SHADER_STORAGE_BUFFER, 4,
    //             //     light_grid.light_index_ssbo);
    //             // glBindBufferBase(
    //             //     GL_SHADER_STORAGE_BUFFER, 5,
    //             //     light_grid.light_span_ssbo);
    //         }

    //         // update_lights(scene, wtv);

    //         // ::render(scene, draw_parameters);

    //         // unbind(scene.program);

    //         delete_(primitive_offsets_ssbo);
    //     }
    // }
    // if(settings.rasterization.frustum_culling.is_anchored) {
    //     auto&& instance = subscribe(wireframe_pass, box_wireframe);
    //     instance->uniforms["color"]
    //     = std::make_shared<eng::Uniform<agl::Vec4>>(
    //         agl::vec4(0.f, 1.f, 0.f, 1.f));
    //     instance->uniforms["model_to_clip"]
    //     = std::make_shared<eng::Uniform<agl::Mat4>>(
    //         wtc * gizmo::box_wireframe_model_to_world(
    //             agl::engine::bounding_box(frustum_culling_camera)));
    // }
    // if(settings.bvh_debugging_enabled) {
    //     traverse(scene_bvh, [&, this](const BvhNode& bn, std::size_t depth) {
    //         auto has_children = bool(bn.inf_node) or bool(bn.sup_node);
    //         if(depth == settings.bvh_debugging_level
    //             or (depth < settings.bvh_debugging_level and not has_children))
    //         {
    //             auto&& instance = subscribe(wireframe_pass, box_wireframe);
    //             instance->uniforms["color"]
    //             = std::make_shared<eng::Uniform<agl::Vec4>>(
    //                 agl::vec4(0.f, 1.f, 0.f, 1.f));
    //             instance->uniforms["model_to_clip"]
    //             = std::make_shared<eng::Uniform<agl::Mat4>>(
    //                 wtc * gizmo::box_wireframe_model_to_world(bn.bounds));
    //         }
    //     });
    // }
    // if(settings.object_grid_debugging_enabled) {
    //     for(auto& cell : scene_grid.cells) {
    //         if(not is_empty(cell)) {
    //             auto&& instance = subscribe(wireframe_pass, box_wireframe);
    //             instance->uniforms["color"]
    //             = std::make_shared<eng::Uniform<agl::Vec4>>(
    //                 agl::vec4(0.f, 1.f, 0.f, 1.f));
    //             instance->uniforms["model_to_clip"]
    //             = std::make_shared<eng::Uniform<agl::Mat4>>(
    //                 wtc * gizmo::box_wireframe_model_to_world(cell.bounds));
    //         }
    //     }
    // }
    // {
    //     agl::engine::render(wireframe_pass);
    // }
    {
        render_ui(*this);
    }
}
