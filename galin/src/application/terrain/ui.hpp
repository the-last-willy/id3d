#pragma once

#include "application.hpp"
#include "reset_terrain.hpp"

void App::ui() {
    if(ImGui::Begin("Settings")) {
        if(ImGui::Button("Reset")) {
            reset_terrain(*this);
        }
        if(ImGui::TreeNode("Algorithm")) {
            if(ImGui::TreeNode("Erosion")) {
                if(ImGui::TreeNode("Debris slope")) {
                    ImGui::DragFloat(
                        "Extent",
                        &settings.debris_slope_erosion_extent,
                        1.f, 0.f, 1000.f,
                        "%.4f",
                        ImGuiSliderFlags_Logarithmic);
                    if(ImGui::Button("Erode")) {
                        debris_slope_erosion(
                            terrain,
                            settings.debris_slope_erosion_extent);
                        settings.is_height_outdated = true;
                    }
                    ImGui::TreePop();
                }
                if(ImGui::TreeNode("Hill slope")) {
                    ImGui::DragFloat(
                        "Extent",
                        &settings.hill_slope_erosion_extent,
                        1.f, 0.f, 1000.f,
                        "%.4f",
                        ImGuiSliderFlags_Logarithmic);
                    if(ImGui::Button("Erode")) {
                        hill_slope_erosion(
                            terrain,
                            settings.hill_slope_erosion_extent);
                        settings.is_height_outdated = true;
                    }
                    ImGui::TreePop();
                }
                if(ImGui::TreeNode("Stream power")) {
                    ImGui::DragFloat(
                        "Extent",
                        &settings.stream_power_erosion_extent,
                        1.f, 0.f, 1000.f,
                        "%.4f",
                        ImGuiSliderFlags_Logarithmic);
                    if(ImGui::Button("Erode")) {
                        stream_power_erosion(
                            terrain,
                            settings.stream_power_erosion_extent);
                        settings.is_height_outdated = true;
                    }
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Breaching")) {
                if(ImGui::Button("Breach")) {
                    breaching::breach(terrain);
                    settings.is_height_outdated = true;
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("View")) {
            ImGui::DragFloat(
                "Height scaling",
                &settings.view_height_scaling,
                1.f, 0.f, 1000.f,
                "%.3f",
                ImGuiSliderFlags_Logarithmic);
            ImGui::DragFloat(
                "Scaling",
                &settings.view_scaling,
                1.f, 0.001f, 1000.f,
                "%.3f",
                ImGuiSliderFlags_Logarithmic);
            ImGui::Checkbox("Show vegetation", &settings.show_vegetation);
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Terrain")) {
            ImGui::Text(
                "Delta = [%.3f, %.3f]",
                delta(terrain)[0],
                delta(terrain)[1]);
            ImGui::Text(
                "Domain = [[%.3f, %.3f], [%.3f, %.3f]]",
                lower_bound(terrain.settings.domain)[0],
                upper_bound(terrain.settings.domain)[0],
                lower_bound(terrain.settings.domain)[1],
                upper_bound(terrain.settings.domain)[1]);
            if(ImGui::TreeNode("Color")) {
                auto items = std::array{
                    "Drainage area",
                    "Height",
                    "Laplacian",
                    "Normals",
                    "Shading",
                    "Slope",
                    "Vegetation probability",
                    "Wetness"};
                auto current = int(settings.color_formula);
                auto used = ImGui::Combo(
                    "Formula",
                    &current,
                    data(items), int(size(items)));
                if(used) {
                    settings.is_color_outdated = true;
                }
                settings.color_formula = ColorFormula(current);
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Drainage area")) {
                ImGui::Text(
                    "Range = [%.3f, %.3f]",
                    lower_bound(terrain.drainage_area.range),
                    upper_bound(terrain.drainage_area.range));
                { // Formula.
                    auto items = std::array{
                        "Steepest", "Weighted"};
                    auto current = int(settings.drainage_area_formula);
                    auto used = ImGui::Combo(
                        "Formula",
                        &current,
                        data(items), int(size(items)));
                    if(used) {
                        settings.is_drainage_area_outdated = true;
                    }
                    settings.drainage_area_formula = DrainageAreaFormula(current);
                }
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Height")) {
                ImGui::Text(
                    "Range = [%.3f, %.3f]",
                    lower_bound(terrain.height.range),
                    upper_bound(terrain.height.range));
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Laplacian")) {
                ImGui::Text(
                    "Range = [%.3f, %.3f]",
                    lower_bound(terrain.laplacian.range),
                    upper_bound(terrain.laplacian.range));
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Slope")) {
                ImGui::Text(
                    "Range = [%.3f, %.3f]",
                    lower_bound(terrain.slope.range),
                    upper_bound(terrain.slope.range));
                { // Formula.
                    auto items = std::array{"Gradient", "Mean"};
                    auto current = int(settings.slope_formula);
                    auto used = ImGui::Combo(
                        "Formula",
                        &current,
                        data(items), int(size(items)));
                    if(used) {
                        settings.is_slope_outdated = true;
                    }
                    settings.slope_formula = SlopeFormula(current);
                }
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Wetness")) {
                ImGui::Text(
                    "Range = [%.3f, %.3f]",
                    lower_bound(terrain.wetness.range),
                    upper_bound(terrain.wetness.range));
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}
