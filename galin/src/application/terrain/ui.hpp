#pragma once

#include "application.hpp"

void App::ui() {
    if(ImGui::Begin("Settings")) {
        if(ImGui::TreeNode("View")) {

            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Erosion")) {
            if(ImGui::TreeNode("Debris slope")) {
                ImGui::SliderFloat("Factor",
                    &settings.debris_slope_erosion_factor, 0.f, 5.f);
                if(ImGui::Button("Erode")) {
                    debris_slope_erosion(terrain);
                    update_all(terrain);
                }
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Hill slope")) {
                ImGui::SliderFloat("Factor",
                    &settings.hill_slope_erosion_factor, 0.f, 5.f);
                if(ImGui::Button("Erode")) {
                    hill_slope_erosion(terrain);
                    update_all(terrain);
                }
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Stream power")) {
                ImGui::SliderFloat("Factor",
                    &settings.stream_power_erosion_factor, 0.f, 5.f);
                if(ImGui::Button("Erode")) {
                    stream_power_erosion(terrain);
                    update_all(terrain);
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}
