#pragma once

#include "application.hpp"

inline
void render_ui(Application& app) {
    if(ImGui::Begin("Settings")) {
        if(ImGui::TreeNode("Frustum culling")) {
            ImGui::Checkbox("Enabled",
                &app.settings.frustum_culling.is_enabled);

            ImGui::NewLine();

            ImGui::Text("Debbuging:");
            ImGui::Checkbox(
                "Anchor view",
                &app.settings.is_culling_anchored);

            ImGui::NewLine();

            ImGui::Text("Statistics:");
            ImGui::Text("Accepted: %i (%f%%)", 
                app.statistics.frustum_culling.accepted_count,
                100.f * accepted_ratio(app.statistics.frustum_culling));
            ImGui::Text("Rejected: %i (%f%%)",
                app.statistics.frustum_culling.rejected_count,
                100.f * rejected_ratio(app.statistics.frustum_culling));
                
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Occlusion culling")) {
            ImGui::Checkbox("Enabled",
                &app.settings.occlusion_culling.is_enabled);

            ImGui::NewLine();

            ImGui::Text("Debbuging:");
            ImGui::Checkbox(
                "Anchor view",
                &app.settings.is_culling_anchored);

            ImGui::NewLine();

            ImGui::Text("Statistics:");
            ImGui::Text("Accepted: %i (%f%%)", 
                app.statistics.occlusion_culling.accepted_count,
                100.f * accepted_ratio(app.statistics.occlusion_culling));
            ImGui::Text("Rejected: %i (%f%%)",
                app.statistics.occlusion_culling.rejected_count,
                100.f * rejected_ratio(app.statistics.occlusion_culling));
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Rasterizer")) {
            ImGui::Checkbox("Enabled",
                &app.settings.rasterizer.is_enabled);
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Ray tracer")) {
            ImGui::Checkbox("Enabled",
                &app.settings.ray_tracer.is_enabled);

            ImGui::NewLine();

            ImGui::Checkbox("Shooting",
                &app.settings.ray_tracer.is_shooting);

            ImGui::NewLine();

            ImGui::Text("Statistics:");
            ImGui::Text("Point count: %i", 
                app.statistics.ray_tracer.point_count);

            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Z-prepass")) {
            ImGui::Checkbox("Enabled",
                &app.settings.z_prepass.is_enabled);
            ImGui::TreePop();
        }
    }
    ImGui::End();
}
