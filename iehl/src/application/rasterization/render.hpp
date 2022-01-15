#pragma once

#include "render_ui.hpp"
#include "transformed.hpp"

#include <agl/engine/all.hpp>

void Application::render() {
    auto v2c = agl::engine::eye_to_clip(camera);
    auto w2c = agl::engine::world_to_clip(camera);
    auto m2c = w2c;
    auto m2w = agl::mat4(agl::identity);

    auto eye_world_position = camera.view.position;

    auto wire_quad_colors = std::vector<agl::Vec4>();
    auto wire_quad_transforms = std::vector<agl::Mat4>();

    auto gizmo_solid_commands = CommandGroup();

    auto accepted_commands = CommandGroup(size(scene.objects.topology.commands));
    auto frustum_culling_rejected_commands = CommandGroup();
    auto occlusion_culling_rejected_commands = CommandGroup();

    { // Setup.
        // Initialize the draw commands with all the draw commands of the scene.
        // Culling passes will filter these.
        accepted_commands.commands = scene.objects.topology.commands.commands;
        copy_cpu_to_gpu(accepted_commands);
    }
    if constexpr(false) { // Frustum culling.
        auto output_commands = CommandGroup(size(accepted_commands));
        copy_cpu_to_gpu(output_commands);

        bind(frustum_culler);
        bind(frustum_culler,
            accepted_commands, output_commands);
        bind(frustum_culler,
            scene.objects.data);

        auto fc_clip_to_world
        = agl::engine::clip_to_world(frustum_culling_camera);
        auto fc_world_to_clip
        = agl::engine::world_to_clip(frustum_culling_camera);

        auto frustum_clip_bounds = agl::common::interval(
            agl::vec4(agl::vec3(-1.f), 1.f),
            agl::vec4(agl::vec3(+1.f), 1.f));
        auto frustum_world_bounds = transformed(
            fc_clip_to_world,
            frustum_clip_bounds);

        glProgramUniform4fv(frustum_culler.program,
            frustum_culler.frustum_clip_bounds_0_uniform_location,
            1, data(lower_bound(frustum_clip_bounds)));
        glProgramUniform4fv(frustum_culler.program,
            frustum_culler.frustum_clip_bounds_1_uniform_location,
            1, data(upper_bound(frustum_clip_bounds)));
        glProgramUniform4fv(frustum_culler.program,
            frustum_culler.frustum_world_bounds_0_uniform_location,
            1, data(lower_bound(frustum_world_bounds)));
        glProgramUniform4fv(frustum_culler.program,
            frustum_culler.frustum_world_bounds_1_uniform_location,
            1, data(upper_bound(frustum_world_bounds)));
        glProgramUniformMatrix4fv(frustum_culler.program,
            frustum_culler.world_to_clip_uniform_location,
            1, GL_FALSE, data(fc_world_to_clip));

        glDispatchCompute((size(accepted_commands) + 255) / 256, 1, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

        copy_gpu_to_cpu(output_commands);

        std::sort(begin(accepted_commands.commands), end(accepted_commands.commands));
        std::sort(begin(output_commands.commands), end(output_commands.commands));

        std::set_difference(
            begin(accepted_commands.commands), end(accepted_commands.commands),
            begin(output_commands.commands), end(output_commands.commands),
            std::back_inserter(frustum_culling_rejected_commands.commands));

        copy_cpu_to_gpu(frustum_culling_rejected_commands);

        statistics.frustum_culling.accepted_count
        = size(output_commands);
        statistics.frustum_culling.rejected_count
        = size(frustum_culling_rejected_commands);

        accepted_commands = std::move(output_commands);
    }
    if constexpr(false) { // Occlusion culling.
        { // Drawing.
            bind_for_drawing(occlusion_culler);

            gl::ClearNamedFramebuffer(occlusion_culler.depth_fbo,
                GL_DEPTH, 0, 1.f);

            glViewport(0, 0, 512, 256);

            glBindVertexArray(forward_rendering_vao);

            glProgramUniformMatrix4fv(occlusion_culler.draw_program,
                occlusion_culler.draw_model_to_clip_uniform_location,
                1, GL_FALSE, data(m2c));

            glDepthFunc(GL_LESS);
            glEnable(GL_DEPTH_TEST);

            glBindBuffer(GL_DRAW_INDIRECT_BUFFER,
                accepted_commands.commands_buffer);

            glMultiDrawElementsIndirect(
                GL_TRIANGLES, GL_UNSIGNED_INT,
                0, GLsizei(size(accepted_commands.commands)), 0);

            glDisable(GL_DEPTH_TEST);
        }
        { // Mipmapping.
            generate_mipmap(occlusion_culler);
        }
        { // Culling.
            auto output_commands = CommandGroup(size(accepted_commands));
            copy_cpu_to_gpu(output_commands);

            bind_for_culling(occlusion_culler);
            bind_for_culling(occlusion_culler,
                accepted_commands, output_commands);
            bind_for_culling(occlusion_culler,
                scene.objects.data);

            auto oc_w2c = agl::engine::world_to_clip(occlusion_culling_camera);

            glProgramUniformMatrix4fv(occlusion_culler.cull_program,
                occlusion_culler.cull_world_to_clip_uniform_location,
                1, GL_FALSE, data(oc_w2c));

            glDispatchCompute((size(accepted_commands) + 255) / 256, 1, 1);
            glMemoryBarrier(GL_ALL_BARRIER_BITS);

            copy_gpu_to_cpu(output_commands);

            std::sort(begin(accepted_commands.commands), end(accepted_commands.commands));
            std::sort(begin(output_commands.commands), end(output_commands.commands));

            // for(auto& c : output_commands.commands) {
            //     std::cout << c.baseInstance << " ";
            // }
            // std::cout << std::endl;

            std::set_difference(
                begin(accepted_commands.commands), end(accepted_commands.commands),
                begin(output_commands.commands), end(output_commands.commands),
                std::back_inserter(occlusion_culling_rejected_commands.commands));

            copy_cpu_to_gpu(occlusion_culling_rejected_commands);

            accepted_commands = std::move(output_commands);
        }
    }
    { // Setup HDR framebuffer.
        bind(hdr_framebuffer);

        gl::ClearNamedFramebuffer(hdr_framebuffer.fbo, GL_DEPTH, 0,
            1.f);
        gl::ClearNamedFramebuffer(hdr_framebuffer.fbo, GL_COLOR, 0,
            std::array{0.f, 0.f, 0.f, 1.f});

        glViewport(0, 0, 1280, 720);
    }
    // Z-Prepass.
    if(settings.z_prepass.is_enabled) {
        bind(z_prepasser);

        glBindVertexArray(scene_z_prepasser_vao);

        glDepthFunc(GL_LESS);
        auto cap_dt = scoped(gl::Enable(GL_DEPTH_TEST));
        
        glProgramUniformMatrix4fv(z_prepasser.program,
            z_prepasser.world_to_clip,
            1, GL_FALSE, data(w2c));

        glBindBuffer(GL_DRAW_INDIRECT_BUFFER,
            accepted_commands.commands_buffer);
        glMultiDrawElementsIndirect(
            GL_TRIANGLES, GL_UNSIGNED_INT,
            0, size(accepted_commands), 0);
    }
    if constexpr(true) { // Drawing.
        bind(forward_renderer);
        bind(forward_renderer, accepted_commands);
        bind(forward_renderer, scene.materials);
        upload(forward_renderer, light_culling);
        upload(forward_renderer, scene.lights);
        upload(forward_renderer, scene.objects.data);

        glBindVertexArray(forward_rendering_vao);

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

        glProgramUniform3fv(forward_renderer.program,
            forward_renderer.light_culling_domain_lower_bounds_location,
            1, data(lower_bound(scene.objects.data.bounds).xyz()));
        glProgramUniform3fv(forward_renderer.program,
            forward_renderer.light_culling_domain_upper_bounds_location,
            1, data(upper_bound(scene.objects.data.bounds).xyz()));

        // glCullFace(GL_BACK);
        // glEnable(GL_CULL_FACE);

        glDepthFunc(GL_LEQUAL);
        glEnable(GL_DEPTH_TEST);

        glBindBuffer(GL_DRAW_INDIRECT_BUFFER,
            accepted_commands.commands_buffer);

        glMultiDrawElementsIndirect(
            GL_TRIANGLES, GL_UNSIGNED_INT,
            0, size(accepted_commands), 0);

        // glDisable(GL_CULL_FACE);

        glDisable(GL_DEPTH_TEST);
    }
    if constexpr(false) { // Solid BBox drawing.
        glUseProgram(solid_renderer.program);

        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);
        auto cap_b = scoped(gl::Enable(GL_BLEND));

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        auto cap_dt = scoped(gl::Enable(GL_DEPTH_TEST));

        glProgramUniform3fv(solid_renderer.program,
            solid_renderer.view_world_position,
            1, data(eye_world_position));
        glProgramUniformMatrix4fv(solid_renderer.program,
            solid_renderer.object_to_clip_position,
            1, GL_FALSE, data(w2c));
        glProgramUniformMatrix4fv(solid_renderer.program,
            solid_renderer.object_to_world_normal,
            1, GL_FALSE, data(agl::mat4(agl::identity)));
        glProgramUniformMatrix4fv(solid_renderer.program,
            solid_renderer.object_to_world_position,
            1, GL_FALSE, data(agl::mat4(agl::identity)));

        glBindVertexArray(solid_box_solid_renderer_vao);

        // Frustum culling rejected commands.

        gl::ProgramUniform4fv(solid_renderer.program,
            solid_renderer.rgba_color,
            std::array{0.2f, 0.f, 0.f, 1.f});

        for(auto& c : frustum_culling_rejected_commands.commands) {
            auto o2w = solid_box_object_to_world(
                scene.objects.data.object_bounds[c.baseInstance]);
            auto o2c = w2c * o2w;
            glProgramUniformMatrix4fv(solid_renderer.program,
                solid_renderer.object_to_clip_position,
                1, GL_FALSE, data(o2c));
            glProgramUniformMatrix4fv(solid_renderer.program,
                solid_renderer.object_to_world_normal,
                1, GL_FALSE, data(o2w));
            glProgramUniformMatrix4fv(solid_renderer.program,
                solid_renderer.object_to_world_position,
                1, GL_FALSE, data(o2w));
            draw(solid_box.topology);
        }

        // Occlusion culling rejected commands.

        gl::ProgramUniform4fv(solid_renderer.program,
            solid_renderer.rgba_color,
            std::array{0.f, 0.2f, 0.f, 1.f});

        for(auto& c : occlusion_culling_rejected_commands.commands) {
            auto o2w = solid_box_object_to_world(
                scene.objects.data.object_bounds[c.baseInstance]);
            auto o2c = w2c * o2w;
            glProgramUniformMatrix4fv(solid_renderer.program,
                solid_renderer.object_to_clip_position,
                1, GL_FALSE, data(o2c));
            glProgramUniformMatrix4fv(solid_renderer.program,
                solid_renderer.object_to_world_normal,
                1, GL_FALSE, data(o2w));
            glProgramUniformMatrix4fv(solid_renderer.program,
                solid_renderer.object_to_world_position,
                1, GL_FALSE, data(o2w));
            draw(solid_box.topology);
        }

        // Accepted commands.

        gl::ProgramUniform4fv(solid_renderer.program,
            solid_renderer.rgba_color,
            std::array{0.f, 0.f, 0.2f, 1.f});

        for(auto& c : accepted_commands.commands) {
            auto o2w = solid_box_object_to_world(
                scene.objects.data.object_bounds[c.baseInstance]);
            auto o2c = w2c * o2w;
            glProgramUniformMatrix4fv(solid_renderer.program,
                solid_renderer.object_to_clip_position,
                1, GL_FALSE, data(o2c));
            glProgramUniformMatrix4fv(solid_renderer.program,
                solid_renderer.object_to_world_normal,
                1, GL_FALSE, data(o2w));
            glProgramUniformMatrix4fv(solid_renderer.program,
                solid_renderer.object_to_world_position,
                1, GL_FALSE, data(o2w));
            draw(solid_box.topology);
        }

        glDepthMask(GL_TRUE);
    }
    if constexpr(false) { // Solid drawing.
        glUseProgram(solid_renderer.program);

        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);
        auto cap_b = scoped(gl::Enable(GL_BLEND));

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        auto cap_dt = scoped(gl::Enable(GL_DEPTH_TEST));

        glProgramUniform3fv(solid_renderer.program,
            solid_renderer.view_world_position,
            1, data(eye_world_position));
        glProgramUniformMatrix4fv(solid_renderer.program,
            solid_renderer.object_to_clip_position,
            1, GL_FALSE, data(w2c));
        glProgramUniformMatrix4fv(solid_renderer.program,
            solid_renderer.object_to_world_normal,
            1, GL_FALSE, data(agl::mat4(agl::identity)));
        glProgramUniformMatrix4fv(solid_renderer.program,
            solid_renderer.object_to_world_position,
            1, GL_FALSE, data(agl::mat4(agl::identity)));

        glBindVertexArray(scene_solid_vao);

        // Frustum culling rejected commands.
        if(size(frustum_culling_rejected_commands) > 0) {
            gl::ProgramUniform4fv(solid_renderer.program,
                solid_renderer.rgba_color,
                std::array{0.2f, 0.f, 0.f, 1.f});
            glBindBuffer(GL_DRAW_INDIRECT_BUFFER,
                frustum_culling_rejected_commands.commands_buffer);
            glMultiDrawElementsIndirect(
                GL_TRIANGLES, GL_UNSIGNED_INT,
                0, size(frustum_culling_rejected_commands), 0);
        }
         // Occlusion culling rejected commands.
        if(size(occlusion_culling_rejected_commands) > 0) {
            gl::ProgramUniform4fv(solid_renderer.program,
                solid_renderer.rgba_color,
                std::array{0.f, 0.2f, 0.f, 1.f});
            glBindBuffer(GL_DRAW_INDIRECT_BUFFER,
                occlusion_culling_rejected_commands.commands_buffer);
            glMultiDrawElementsIndirect(
                GL_TRIANGLES, GL_UNSIGNED_INT,
                0, size(occlusion_culling_rejected_commands), 0);
        }
        // Accepted commands.
        if(size(accepted_commands) > 0) {
            gl::ProgramUniform4fv(solid_renderer.program,
                solid_renderer.rgba_color,
                std::array{0.f, 0.f, 0.2f, 1.f});
            glBindBuffer(GL_DRAW_INDIRECT_BUFFER,
                accepted_commands.commands_buffer);
            glMultiDrawElementsIndirect(
                GL_TRIANGLES, GL_UNSIGNED_INT,
                0, size(accepted_commands), 0);
        }

        glDepthMask(GL_TRUE);
    }
    if constexpr(true) { // Gizmos.
        bind(hdr_framebuffer);

        glUseProgram(solid_renderer.program);

        glBindVertexArray(solid_box_solid_renderer_vao);

        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);
        auto cap_b = scoped(gl::Enable(GL_BLEND));

        glDepthFunc(GL_LESS);
        glDepthMask(GL_FALSE);
        auto cap_dt = scoped(gl::Enable(GL_DEPTH_TEST));

        glCullFace(GL_BACK);
        auto cap_cf = scoped(gl::Enable(GL_CULL_FACE));

        // Frustum culling frustum.
        if(settings.frustum_culling.is_anchored) {
            gl::ProgramUniform4fv(solid_renderer.program,
                solid_renderer.rgba_color,
                std::array{0.5f, 0.f, 0.f, 1.f});
            auto frustum_o2w = solid_box_model_to_world(frustum_culling_camera);
            auto frustum_o2c = w2c * frustum_o2w;
            glProgramUniformMatrix4fv(solid_renderer.program,
                solid_renderer.object_to_clip_position,
                1, GL_FALSE, data(frustum_o2c));
            glProgramUniformMatrix4fv(solid_renderer.program,
                solid_renderer.object_to_world_normal,
                1, GL_FALSE, data(frustum_o2w));
            glProgramUniformMatrix4fv(solid_renderer.program,
                solid_renderer.object_to_world_position,
                1, GL_FALSE, data(frustum_o2w));
            draw(solid_box.topology);
        }
        // Occlusion culling frustum.
        if(settings.occlusion_culling.is_anchored) {
            gl::ProgramUniform4fv(solid_renderer.program,
                solid_renderer.rgba_color,
                std::array{0.f, 0.2f, 0.f, 1.f});
            auto frustum_o2w = solid_box_model_to_world(occlusion_culling_camera);
            auto frustum_o2c = w2c * frustum_o2w;
            glProgramUniformMatrix4fv(solid_renderer.program,
                solid_renderer.object_to_clip_position,
                1, GL_FALSE, data(frustum_o2c));
            glProgramUniformMatrix4fv(solid_renderer.program,
                solid_renderer.object_to_world_normal,
                1, GL_FALSE, data(frustum_o2w));
            glProgramUniformMatrix4fv(solid_renderer.program,
                solid_renderer.object_to_world_position,
                1, GL_FALSE, data(frustum_o2w));
            draw(solid_box.topology);
        }
        glCullFace(GL_FRONT);
        // Lights.
        // for(auto& l : scene.lights.light_properties) {
        //     auto l_o2w = agl::translation(l.position.xyz()) * agl::scaling3(0.5f);
        //     auto l_o2c = w2c * l_o2w;
        //     gl::ProgramUniform4fv(solid_renderer.program,
        //         solid_renderer.rgba_color,
        //         1, data(l.rgb_color));
        //     glProgramUniformMatrix4fv(solid_renderer.program,
        //         solid_renderer.object_to_clip_position,
        //         1, GL_FALSE, data(l_o2c));
        //     glProgramUniformMatrix4fv(solid_renderer.program,
        //         solid_renderer.object_to_world_normal,
        //         1, GL_FALSE, data(l_o2w));
        //     glProgramUniformMatrix4fv(solid_renderer.program,
        //         solid_renderer.object_to_world_position,
        //         1, GL_FALSE, data(l_o2w));
        //     draw(solid_box.topology);
        // }

        glDepthMask(GL_TRUE);
    }
    { // Tone mapping.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        bind(tone_mapper);
        bind(tone_mapper, hdr_framebuffer);

        glViewport(0, 0, 1280, 720);

        glBindVertexArray(tone_mapping_vao);

        draw(tone_mapper);
    }
    {
        render_ui(*this);
    }
}

