#pragma once

#include "application.hpp"

void App::ui() {
    if(ImGui::Begin("Settings")) {
        if(ImGui::TreeNode("Erosion")) {
            if(ImGui::TreeNode("Debris slope")) {
                ImGui::SliderFloat("Factor",
                    &settings.debris_slope_erosion_factor, 0.f, 5.f);
                if(ImGui::Button("Erode")) {
                    debris_slope_erosion(terrain);
                    settings.is_height_outdated = true;
                }
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Hill slope")) {
                ImGui::SliderFloat("Factor",
                    &settings.hill_slope_erosion_factor, 0.f, 5.f);
                if(ImGui::Button("Erode")) {
                    hill_slope_erosion(terrain);
                    settings.is_height_outdated = true;
                }
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Stream power")) {
                ImGui::SliderFloat("Factor",
                    &settings.stream_power_erosion_factor, 0.f, 5.f);
                if(ImGui::Button("Erode")) {
                    stream_power_erosion(terrain);
                    settings.is_height_outdated = true;
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Field")) {
            if(ImGui::TreeNode("Color")) {
                auto items = std::array{"Laplacian", "Normals", "Slope"};
                auto current = int(settings.color_formula);
                auto used = ImGui::Combo(
                    "Field",
                    &current,
                    data(items), int(size(items)));
                if(used) {
                    settings.is_color_outdated = true;
                }
                settings.color_formula = ColorFormula(current);
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Drainage area")) {
                auto items = std::array{"Derivative"};
                auto current = int(settings.drainage_area_formula);
                auto used = ImGui::Combo(
                    "Slope formula",
                    &current,
                    data(items), int(size(items)));
                if(used) {
                    settings.is_drainage_area_outdated = true;
                }
                settings.drainage_area_formula = DrainageAreaFormula(current);
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Slope")) {
                auto items = std::array{"Gradient"};
                auto current = int(settings.slope_formula);
                auto used = ImGui::Combo(
                    "Slope formula",
                    &current,
                    data(items), int(size(items)));
                if(used) {
                    settings.is_slope_outdated = true;
                }
                settings.slope_formula = SlopeFormula(current);
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}
