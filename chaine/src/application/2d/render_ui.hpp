#pragma once

inline
void App::render_ui() {
    if(ImGui::BeginMainMenuBar()) {
        ImGui::MenuItem("Settings", NULL, &settings.show_settings);
        ImGui::MenuItem("Help", NULL, &settings.show_help);
    }
    ImGui::EndMainMenuBar();
    if(settings.show_help) {
        if(ImGui::Begin("Help")) {
            ImGui::Text("2D mode: ...");
            ImGui::Text("3D mode: ...");
        }
        ImGui::End();
    }
    if(settings.show_settings) {
        if(ImGui::Begin("Settings", &settings.show_settings)) {
            if(ImGui::TreeNode("View")) {
                ImGui::Text("View mode: ");
                ImGui::SameLine();
                if(ImGui::RadioButton("2D", settings.view_mode == ViewMode::_2D)) {
                    settings.view_mode = ViewMode::_2D;
                }
                ImGui::SameLine();
                if(ImGui::RadioButton("3D", settings.view_mode == ViewMode::_3D)) {
                    settings.view_mode = ViewMode::_3D;
                }
                ImGui::NewLine();
                ImGui::Checkbox("Show edges",
                    &settings.show_edges);
                ImGui::Checkbox("Show triangles",
                    &settings.show_triangles);
                ImGui::Checkbox("Show vertices",
                    &settings.show_vertices);
                ImGui::NewLine();
                ImGui::Checkbox("Show triangle connectivity",
                    &settings.show_triangle_connectivity);
                ImGui::Checkbox("Show voronoi",
                    &settings.show_voronoi);
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Action")) {
                if(ImGui::Button("Make Delaunay")) {
                    lawson(mesh);
                    refresh_mesh();
                }
                ImGui::SameLine();
                if(ImGui::Button("Single step")) {
                    lawson_once(mesh);
                    refresh_mesh();
                }
                if(ImGui::Button("Ruppert once")) {
                    ruppert_once(RuppertSettings(), mesh);
                    refresh_mesh();
                }
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Insertion")) {
                ImGui::Checkbox("Lawson",
                    &settings.delaunay_insertion);
                ImGui::Checkbox("Ruppert",
                    &settings.show_voronoi);
                ImGui::TreePop();
            }
           
            ImGui::Checkbox("Delaunay insertion",
                &settings.delaunay_insertion);
            ImGui::NewLine();
            ImGui::Checkbox("Show voronoi",
                &settings.show_voronoi);

            ImGui::Checkbox("Show vertices", &render_settings.show_vertices);

            ImGui::SliderFloat("Point size", &render_settings.point_size, 1.0f, 10.0f);

            ImGui::Separator();

            ImGui::Checkbox("Show edges", &render_settings.show_edges);

            ImGui::SliderFloat("Line width", &render_settings.line_width, 1.0f, 10.0f);

            ImGui::Separator();

            ImGui::Checkbox("Show triangles", &render_settings.show_triangles);

            auto render_modes = std::array{"flat shading", "mean curvature", "smooth normal"};
            ImGui::Combo("Render mode", &render_settings.selected_render_mode,
                std::data(render_modes), static_cast<int>(size(render_modes)));

            
        }
        ImGui::End(); 
    }
}
