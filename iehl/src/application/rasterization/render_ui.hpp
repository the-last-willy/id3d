#pragma once

#include "application.hpp"

inline
void render_ui(Application& app) {
    if(ImGui::Begin("Settings")) {
        if(ImGui::TreeNode("Rasterization")) {
            ImGui::Checkbox(
                "Enabled",
                &app.settings.rasterization_enabled);
            if(ImGui::TreeNode("Frustum culling")) {
                {
                    auto used = ImGui::RadioButton(
                        "Disabled",
                        app.settings.rasterization.frustum_culling.mode == FrustumCullingMode::disabled);
                    if(used) {
                        app.settings.rasterization.frustum_culling.mode = FrustumCullingMode::disabled;
                    }
                }
                ImGui::SameLine();
                {
                    auto used = ImGui::RadioButton(
                        "CPU",
                        app.settings.rasterization.frustum_culling.mode == FrustumCullingMode::cpu);
                    if(used) {
                        app.settings.rasterization.frustum_culling.mode = FrustumCullingMode::cpu;
                    }
                }
                ImGui::SameLine();
                {
                    auto used = ImGui::RadioButton(
                        "GPU",
                        app.settings.rasterization.frustum_culling.mode == FrustumCullingMode::gpu);
                    if(used) {
                        app.settings.rasterization.frustum_culling.mode = FrustumCullingMode::gpu;
                    }
                }
                if(ImGui::TreeNode("Statistics")) {
                    ImGui::Text("Accepted: %i (%f%%)", 
                        app.statistics.frustum_culling.accepted_count,
                        100.f * accepted_ratio(app.statistics.frustum_culling));
                    ImGui::Text("Rejected: %i (%f%%)",
                        app.statistics.frustum_culling.rejected_count,
                        100.f * rejected_ratio(app.statistics.frustum_culling));
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("BVH")) {
            ImGui::Checkbox(
                "Enabled",
                &app.settings.bvh_debugging_enabled);
            ImGui::SliderInt(
                "Level",
                &app.settings.bvh_debugging_level,
                0, 10);
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Object grid")) {
            ImGui::Checkbox(
                "Enabled",
                &app.settings.object_grid_debugging_enabled);
            ImGui::TreePop();
        }
    }
    ImGui::End();
}
