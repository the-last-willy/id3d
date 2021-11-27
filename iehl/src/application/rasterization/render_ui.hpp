#pragma once

#include "application.hpp"

inline
void render_ui(Application& app) {
    if(ImGui::Begin("Settings")) {
        if(ImGui::TreeNode("Rasterization")) {
            ImGui::Checkbox(
                "Enabled",
                &app.settings.rasterization_enabled);
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
