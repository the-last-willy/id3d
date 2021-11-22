void GltfProgram::render() {
    clear(agl::default_framebuffer, agl::depth_tag, 1.f);

    auto vp_tr = agl::engine::world_to_clip(*camera);
    auto v_tr = agl::engine::world_to_eye(*camera);
    auto normal_tr = agl::engine::normal_transform(*camera);

    auto light_position = (agl::vec4(0.f, 0.f, 0.f, 1.f)).xyz();
    auto view_position = (vec4(camera->view.position, 1.f)).xyz();

    if constexpr(false) {
        for(auto& s : ambient_pass.subscriptions) {
            s.mesh->uniforms["mvp_transform"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(vp_tr);
        }
    }
    if constexpr(true) {
        for(auto& s : blinn_phong_pass.subscriptions) {
            s.mesh->uniforms["mvp_transform"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(vp_tr);
        }
    }
    // if constexpr(false) { // Frustrum culling.
    //     auto frustrum = agl::engine::bounding_box(*camera);
        
    //     int count = 0;
    //     for(auto& s : ambient_pass.subscriptions) {
    //         auto bb = bounding_box(*s.mesh);
    //         s.mesh->enabled = false;
    //         if(are_intersecting(bb, frustrum)) {
    //             s.mesh->enabled = true;
    //             count += 1;
    //         }
    //     }
    // }
    if(toggle_rasterization) {
        if constexpr(false) {
            if(ambient_pass_loaded) { // Ambient pass.
                agl::engine::render(ambient_pass);
            }
        }
        if constexpr(false) {
            if(blinn_phong_pass_loaded) { // Blinn Phong pass.
                blinn_phong_pass.uniforms["light_position"]
                = std::make_shared<eng::Uniform<agl::Vec3>>(light_position);
                blinn_phong_pass.uniforms["normal_transform"]
                = std::make_shared<eng::Uniform<agl::Mat4>>(normal_tr);
                blinn_phong_pass.uniforms["view_position"]
                = std::make_shared<eng::Uniform<agl::Vec3>>(view_position);
                agl::engine::render(blinn_phong_pass);
            }
        }
    }
    

    if constexpr(false) { // Point pass.
        point_pass.uniforms["mvp_transform"]
        = std::make_shared<eng::Uniform<agl::Mat4>>(vp_tr);
        point_pass.uniforms["world_to_eye"]
        = std::make_shared<eng::Uniform<agl::Mat4>>(v_tr);
        agl::engine::render(point_pass);
    }

    if constexpr(true) { // Wireframe pass.
        agl::engine::render(wireframe_pass);
    }

    { // UI
        if(ImGui::BeginMainMenuBar()) {
            ImGui::MenuItem("Settings", NULL, &settings.ui.show_settings);
            ImGui::EndMainMenuBar();
        }
        if(settings.ui.show_settings and ImGui::Begin("Settings", &settings.ui.show_settings)) {
            if(ImGui::CollapsingHeader("Rasterization")) {
                if(ImGui::TreeNode("Optimization")) {
                    if(ImGui::TreeNode("Rendering")) {
                        ImGui::TreePop();
                    }
                    if(ImGui::TreeNode("Z-prepass")) {
                        ImGui::TreePop();
                    }
                    if(ImGui::TreeNode("Z-sorting")) {
                        ImGui::TreePop();
                    }
                    ImGui::Separator();
                    if(ImGui::TreeNode("Frustrum culling")) {
                        ImGui::Checkbox("Enabled",
                            &settings.frustrum_culling.enabled);
                        ImGui::NewLine();
                        ImGui::Text("Statistics:");
                        ImGui::Text("Accepted: %i (%f%%)", 
                            statistics.frustrum_culling.accepted_count,
                            accepted_percentage(statistics.frustrum_culling));
                        ImGui::Text("Rejected: %i (%f%%)",
                            statistics.frustrum_culling.rejected_count,
                            rejected_percentage(statistics.frustrum_culling));
                        ImGui::Text("Total: %i",
                            total_count(statistics.frustrum_culling));
                        ImGui::NewLine();
                        ImGui::Text("Debugging:");
                        ImGui::Checkbox("Anchor frustrum",
                            &settings.debugging.is_frustrum_anchored);
                        ImGui::Checkbox("Draw rejected bounding boxes",
                            &settings.frustrum_culling.draw_rejected_bounding_boxes);
                        ImGui::Checkbox("Draw rejected objects",
                            &settings.frustrum_culling.draw_rejected_objects);
                        ImGui::TreePop();
                    }
                    if(ImGui::TreeNode("Occlusion culling")) {
                        ImGui::TreePop();
                    }
                    ImGui::Separator();
                    ImGui::TreePop();
                }
                if(ImGui::TreeNode("Statistics")) {
                    ImGui::TreePop();
                }
            }
            if(ImGui::CollapsingHeader("Ray tracing")) {
                ImGui::DragFloat("Range",
                &ray_tracing_settings.range,
                0.01f, 0.f, 1, "%.3f %%", ImGuiSliderFlags_AlwaysClamp);
                {
                    auto i = int(ray_tracing_settings.ray_per_frame);
                    ImGui::DragInt("Ray per frame",
                        &i,
                        1, 0, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
                    ray_tracing_settings.ray_per_frame = uint32_t(i);
                }
            }
            
            ImGui::End();
        }

        ImGui::Begin("Camera");

        ImGui::InputFloat("X", &camera->view.position[0], 0.f, 0.f, "%.3f");
        ImGui::InputFloat("Y", &camera->view.position[1], 0.f, 0.f, "%.3f");
        ImGui::InputFloat("Z", &camera->view.position[2], 0.f, 0.f, "%.3f");
        ImGui::End();
    }
}
