#pragma once

#include "application.hpp"

void App::ui() {
    if(ImGui::BeginMainMenuBar()) {
        ImGui::MenuItem("Settings", NULL, &settings.show_settings);
    }
    ImGui::EndMainMenuBar();
    if(ImGui::Begin("Settings")) {
        if(ImGui::TreeNode("View")) {
            ImGui::Checkbox("Show bounding box",
                &settings.show_bounding_box);
            
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Bezier patches")) {
            if(ImGui::Button("Randomize colors")) {
                auto d01 = std::uniform_real_distribution<float>(0.f, 1.f);
                using agl::constant::tau;
                for(auto& p : object.patches) {
                    auto t = d01(random_generator);
                    auto a = agl::vec3(0.5f);
                    auto b = agl::vec3(0.5f);
                    auto c = agl::vec3(1.f);
                    auto d = agl::vec3(0.f, 1.f / 3.f, 2.f / 3.f);
                    auto rgb = a + b * agl::cos(tau * (c * t + d));
                    p.color = agl::vec4(rgb, 1.f);
                    p.gpu_tesselation->uniforms["color_factor"]
                    = std::make_shared<eng::Uniform<agl::Vec4>>(p.color);
                }
            }
            if(ImGui::Button("Reset colors")) {
                for(auto& p : object.patches) {
                    p.color = agl::vec4(agl::vec3(1.f), 1.f);
                    p.gpu_tesselation->uniforms["color_factor"]
                    = std::make_shared<eng::Uniform<agl::Vec4>>(p.color);
                }
            }
            ImGui::NewLine();
            ImGui::Checkbox("Show tessellation",
                &settings.show_triangulation);
            ImGui::Checkbox("Show control points",
                &settings.show_control_points);
            ImGui::NewLine();
            ImGui::SliderInt("Resolution",
                &settings.tesselation_resolution, 2, 30);
            ImGui::NewLine();
            // for(std::size_t i = 0; i < size(object.patches); ++i) {
            //     auto& patch = object.patches[i];
            //     if(ImGui::TreeNode(("#" + std::to_string(i)).c_str())) {
            //         if(ImGui::SliderFloat3(("Color##" + std::to_string(i)).c_str(), data(patch.color), 0.f, 1.f)) {
            //             patch.gpu_tesselation->uniforms["color_factor"]
            //             = std::make_shared<eng::Uniform<agl::Vec4>>(patch.color);
            //         }
            //         ImGui::TreePop();
            //     }
            // }
            // ImGui::NewLine();
            if(ImGui::Button("Update")) {
                update_object();
            }
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Revolution surface")) {
            auto& rs = object.revolution_surface;
            ImGui::Checkbox("Show tessellation", &settings.show_revolution);
            ImGui::Checkbox("Show control points", &settings.show_rev_control_curve);
            ImGui::NewLine();
            auto cp_count = int(size(rs.revolution_curve, 0));
            if(ImGui::SliderInt("# of control points", &cp_count, 2, 10)) {
                rs.revolution_curve = agl::common::Grid<agl::Vec3>(
                    agl::common::grid_indexing(std::size_t(cp_count)));
                for(auto& e : rs.revolution_curve) {
                    e = agl::vec3(0.f, 0.f, 1.f);
                }
                update_control_mesh(rs);
            }
            if(ImGui::TreeNode("Control points")) {
                ImGui::Text("X/Y/Weight:");
                for(std::size_t i = 0; i < size(rs.revolution_curve, 0); ++i) {
                    if(ImGui::DragFloat3(
                        ("#" + std::to_string(i)).c_str(),
                        data(at(rs.revolution_curve, i)),
                        0.1f))
                    {
                        update_control_mesh(rs);
                    }
                }
                ImGui::TreePop();
            }
            ImGui::NewLine();
            auto axial_resolution = int(rs.axial_resolution);
            ImGui::SliderInt(
                "Axial resolution",
                &axial_resolution, 2, 30);
            rs.axial_resolution = std::size_t(axial_resolution);
            auto radial_resolution = int(rs.radial_resolution);
            ImGui::SliderInt(
                "Radial resolution",
                &radial_resolution, 2, 30);
            rs.radial_resolution = std::size_t(radial_resolution);
            ImGui::NewLine();
            if(ImGui::Button("Update")) {
                update_object();
            }
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Transform")) {
            ImGui::Checkbox("Enabled", &settings.transform_enabled);
            ImGui::NewLine();

            ImGui::Combo("Area of effect", &settings.transform_attenuation, "none\0linear\0cubic\0\0");
            ImGui::Combo("Type", &settings.transform_type, "none\0attract\0bend\0twist\0\0");
            ImGui::NewLine();

            ImGui::SliderFloat("Intensity",
                &settings.transform_intensity, 0.f, 1.f);
            ImGui::DragFloat3("Position", 
                data(settings.transform_position), 0.05f);
            ImGui::SliderFloat("Radius",
                &settings.transform_radius, 0.f, 5.f);

            ImGui::NewLine();

            ImGui::Checkbox("Show gizmo",
                &settings.show_gizmo);
            ImGui::NewLine();
            if(ImGui::Button("Update")) {
                update_object();
            }
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Free form deformation")) {
            ImGui::Checkbox("Enabled", &settings.ffd_enabled);

            ImGui::NewLine();

            ImGui::Checkbox("Show control grid", &settings.ffd_show_control_grid);

            ImGui::NewLine();

            ImGui::SliderInt3("Resolution",
                data(settings.ffd.resolution), 2, 4);
            if(ImGui::Button("Generate")) {
                generate_ffd();
            }

            ImGui::NewLine();

            if(ImGui::TreeNode("Points")) {
                if(ffd) {
                    auto updated = false;
                    auto& f = *ffd;
                    for(std::size_t i = 0; i < size(f.control_points, 0); ++i)
                    for(std::size_t j = 0; j < size(f.control_points, 1); ++j)
                    for(std::size_t k = 0; k < size(f.control_points, 2); ++k) {
                        auto s = "(" + std::to_string(i) + ", " + std::to_string(j) + ", " + std::to_string(k) + ")";
                        updated |= ImGui::DragFloat3(s.c_str(),
                            data(at(f.control_points, i, j, k)), 0.05f);
                    }
                    if(updated) {
                        ffd_control_mesh = agl::standard::shared(
                            agl::engine::instance(
                                agl::engine::wireframe(
                                    control_mesh3(ffd->control_points))));
                    }
                } else {
                    ImGui::Text("You need to generate them first.");
                }
                ImGui::TreePop();
            }
            ImGui::NewLine();
            if(ImGui::Button("Update")) {
                update_object();
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}
