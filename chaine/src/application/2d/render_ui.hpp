#pragma once

inline
void App::render_ui() {
    if(ImGui::BeginMainMenuBar()) {
        ImGui::MenuItem("Settings", NULL, &settings.show_settings);
        ImGui::MenuItem("Help", NULL, &settings.show_help);
    }
    ImGui::EndMainMenuBar();
    if(settings.show_settings) {
        if(ImGui::Begin("Settings", &settings.show_settings)) {
            if(ImGui::TreeNode("View")) {
                ImGui::Checkbox("Show edges",
                    &settings.show_edges);
                ImGui::NewLine();
                ImGui::Checkbox("Show triangle connectivity",
                    &settings.show_triangle_connectivity);
                ImGui::Checkbox("Show voronoi",
                    &settings.show_voronoi);
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Action")) {
                if(ImGui::Button("Lawson")) {
                    lawson(mesh);
                    refresh_mesh();
                }
                if(ImGui::Button("Ruppert")) {
                    ruppert_once(RuppertSettings(), mesh);
                    refresh_mesh();
                }
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Insertion")) {
                ImGui::Checkbox("Lawson",
                    &settings.delaunay_insertion);
                ImGui::TreePop();
            }
        }
        ImGui::End(); 
    }
}
