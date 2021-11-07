// Disabled warnings.

#pragma warning(disable : 4005 4996)

// Local headers.

#include "settings.hpp"
#include "ray/all.hpp"
#include "settings/all.hpp"
#include "statistics/all.hpp"

#include <agl/format/gltf2/all.hpp>
#include <agl/format/wavefront/all.hpp>
#include <agl/standard/all.hpp>
#include <id3d/common/all.hpp>

// External libraries.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <range/v3/view/indirect.hpp>
#include <range/v3/view/map.hpp>

// Standard library.

#include <iostream>
#include <random>

//

inline
auto create_random_generator() {
    auto rd = std::random_device();
    return std::default_random_engine(rd());
}

struct Object {
    std::shared_ptr<agl::engine::TriangleMesh> cpu_mesh;
    std::shared_ptr<eng::Mesh> gpu_mesh;

    // Model space.
    agl::engine::BoundingBox bounding_box;
};

struct RasterizationScene {
    std::vector<Object> objects;
    std::vector<std::shared_ptr<eng::Material>> materials;
};

struct GltfProgram : Program {
    eng::ShaderCompiler shader_compiler = {};

    RasterizationScene rasterization_scene;

    // Active camera.
    std::shared_ptr<eng::Camera> camera = {};
    std::shared_ptr<eng::Camera> frustrum_camera = {};

    bool ambient_pass_loaded = false;
    agl::engine::RenderPass ambient_pass;
    bool blinn_phong_pass_loaded = false;
    agl::engine::RenderPass blinn_phong_pass;

    agl::engine::RenderPass point_pass;
    
    float time = 0.f;

    RayTracer ray_tracer;
    Scene scene;

    int ray_n = 0;

    agl::engine::TriangleMesh inter_mesh;

    bool toggle_rasterization = true;
    
    std::default_random_engine random_generator = create_random_generator();

    RayTracingSettings ray_tracing_settings;

    agl::engine::RenderPass wireframe_pass;

    std::shared_ptr<eng::Mesh> bounding_box_gizmo;

    AllSettings settings;
    AllStatistics statistics;

    void reload_shaders() {
        try {
            ambient_pass.program = std::make_shared<eng::Program>(
                data::wavefront::forward_ambient_program(shader_compiler));
            ambient_pass_loaded = true;
        } catch(...) {
            ambient_pass.program.reset();
            std::cerr << "AMBIENT SHADERS FAILED TO COMPILE." << std::endl << std::endl;
            ambient_pass_loaded = false;
        }
        try {
            blinn_phong_pass.program = std::make_shared<eng::Program>(
                data::wavefront::forward_blinn_phong_program(shader_compiler));
            blinn_phong_pass_loaded = true;
        } catch(...) {
            blinn_phong_pass.program.reset();
            std::cerr << "BLINN PHONG SHADERS FAILED TO COMPILE." << std::endl << std::endl;
            blinn_phong_pass_loaded = false;
        }
    }

    bool new_mesh = true;

    void reload_points() {
        if(vertex_count(inter_mesh) > 100'000) {
            // If enough points. Leave the previous mesh and work on a new one.
            inter_mesh = agl::engine::TriangleMesh();
            new_mesh = true;
        } else {
            if(not new_mesh) {
                point_pass.subscriptions.pop_back();
            }
            new_mesh = false;
            auto render_inter_mesh = std::make_shared<eng::Mesh>(
                agl::engine::point_mesh(inter_mesh, rasterization_scene.materials));
            subscribe(point_pass, render_inter_mesh);
        }
    }

    void init() override {
        { // Shader compiler.
            shader_compiler.log_folder = "logs/";
            shader_compiler.root = "iehl/src/shader";
        }
        { // Read objects from file.
            auto content = agl::format::wavefront::load(
                // "D:/data/cornell-box/cornell-box.obj",
                // "D:/data/cornell-box");
                // "C:/Users/Willy/Desktop/data/wavefront/CornellBox/cornell-box.obj",
                // "C:/Users/Willy/Desktop/data/wavefront/CornellBox");
                "D:/data/bistro-small/exterior.obj",
                "D:/data/bistro-small/");
                // "C:/Users/Willy/Desktop/data/bistro-small/exterior.obj",
                // "C:/Users/Willy/Desktop/data/bistro-small/");
                // "C:/Users/yoanp/Documents/bistro-small/exterior.obj",
                // "C:/Users/yoanp/Documents/bistro-small/");
            for(std::size_t i = 0; i < size(content.meshes); ++i) {
                auto& o = rasterization_scene.objects.emplace_back();
                o.cpu_mesh = content.tmeshes[i];
                o.gpu_mesh = content.meshes[i];
                o.bounding_box = bounding_box(*o.cpu_mesh);
            }
            rasterization_scene.materials = std::move(content.materials);
        }

        { // Normalize data.
            auto default_albedo = std::make_shared<eng::Texture>(
                data::uniform_texture(agl::vec3(1.f)));
            auto default_emissive = std::make_shared<eng::Texture>(
                data::uniform_texture(agl::vec3(0.f)));
            auto default_specular = std::make_shared<eng::Texture>(
                data::uniform_texture(agl::vec3(0.f, 1.f, 0.f)));
            for(auto& o : rasterization_scene.objects) {
                for(auto&& p : o.gpu_mesh->primitives | ranges::views::indirect) {
                    if(p.material) {
                        auto&& ma = *p.material;
                        if(not ma.textures.contains("map_Kd")) {
                            ma.textures["map_Kd"] = default_albedo;
                        }
                        if(not ma.textures.contains("map_Ke")) {
                            ma.textures["map_Ke"] = default_emissive;
                        }
                        if(not ma.textures.contains("map_Ks")) {
                            ma.textures["map_Ks"] = default_specular;
                        }
                    }
                }
            }
        }
        if constexpr(false) {
            // auto& tmesh = *database.tmeshes.front();
            // for(uint32_t fi = 0; fi < face_count(tmesh); ++fi) {
            //     scene.all_faces.push_back(face(tmesh, fi));
            // }
            // for(auto& f : scene.all_faces) {
            //     for(uint32_t vi = 0; vi < incident_vertex_count(f); ++vi) {
            //         auto mid = geometry(tmesh).vertex_material_ids[index(incident_vertex(f, vi))];
            //         if(mid == 0) { // Light material.
            //             scene.emissive_faces.push_back(f);
            //         }
            //     }
            // }
        }

        { // Render passes.
            reload_shaders();
        }

        { // Camera.
            auto& c = *(camera = std::make_shared<eng::Camera>());
            if(auto pp = std::get_if<eng::PerspectiveProjection>(&c.projection)) {
                pp->aspect_ratio = 16.f / 9.f;
                pp->z_far = 100.f;
            }
            frustrum_camera = std::make_shared<eng::Camera>(*camera);
        }
        { // Ray tracer.
            ray_tracer.x_distribution = std::uniform_real_distribution<float>(-1.f, 1.f);
            ray_tracer.y_distribution = std::uniform_real_distribution<float>(-1.f, 1.f);
            ray_tracer.random = &random_generator;
        }
        { // Point pass.
            assign_program(point_pass,
                data::wavefront::point_program(shader_compiler));
        }
        { // Wireframe pass.
            assign_program(wireframe_pass, 
                data::shader::wireframe(shader_compiler));
        }
        { // Gizmos.
            bounding_box_gizmo = std::make_shared<eng::Mesh>(
                agl::engine::wireframe(data::gizmo::bounding_box()));
        }
    }

    void update(float dt) override {
        { // Increment time.
            time += dt;
        }
        if(not ImGui::GetIO().WantCaptureMouse) {
            if(glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_1)) {
                glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                agl::Vec2 d = current_cursor_pos - previous_cursor_pos;
                camera->view.yaw -= d[0] / 500.f;
                camera->view.pitch -= d[1] / 500.f;

                previous_cursor_pos = current_cursor_pos;
            } else {
                glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                previous_cursor_pos = current_cursor_pos;
            }
        }
        { // Camera controls.
            if(glfwGetKey(window.window, GLFW_KEY_A)) {
                auto direction = (rotation(camera->view)
                * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
                camera->view.position = camera->view.position - direction / 10.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_D)) {
                auto direction = (rotation(camera->view)
                * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
                camera->view.position = camera->view.position + direction / 10.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_S)) {
                auto direction = rotation(camera->view)[2].xyz();
                camera->view.position = camera->view.position + direction / 10.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_W)) {
                auto direction = rotation(camera->view)[2].xyz();
                camera->view.position = camera->view.position - direction / 10.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_R)) {
                reload_shaders();
            }
        }

        if(glfwGetKey(window.window, GLFW_KEY_SPACE)) {
            toggle_rasterization = not toggle_rasterization;
        }
        if(ray_tracing_settings.ray_per_frame > 0){
            rtx(); // on
            reload_points();
        }
        if(not settings.debugging.is_frustrum_anchored) {
            *frustrum_camera = *camera;
        }
        update_render_passes();
    }

    void rtx() {
        auto ro = camera->view.position;
        
        auto distrib01 = std::uniform_real_distribution<float>(0.f, 1.f);
        auto norm_distrib = std::uniform_real_distribution<float>(-1.f, 1.f);

        float n = float(ray_tracing_settings.ray_per_frame);
        for(float i = 0; i < n; ++i) {    

            auto x = norm_distrib(random_generator) * ray_tracing_settings.range;
            auto y = norm_distrib(random_generator) * ray_tracing_settings.range;

            auto rd = (agl::engine::rotation(*camera) * agl::vec4(x, y, -1.f, 0.f)).xyz(); 

            auto r = agl::engine::Ray(ro, rd);
            
            auto inter = ray_trace(scene, r);
            if(inter) {
                auto v = create_vertex(inter_mesh);
                position(v) = inter->hit.position;
                auto ivert = incident_vertex(inter->triangle, 0);
                normal(v) = normal(ivert);
                auto lightened = false;
                auto light_f = scene.emissive_faces.front();
                auto light_p = agl::vec3(0.f);
                {
                    auto r0 = distrib01(random_generator);
                    auto r1 = distrib01(random_generator);
                    if(r0 + r1 > 1.f) {
                        r0 = 1.f - r0;
                        r1 = 1.f - r1;
                    }
                    light_p = r0 * position(incident_vertex(light_f, 0))
                    + r1 * position(incident_vertex(light_f, 1))
                    + (1.f - r0 - r1) * position(incident_vertex(light_f, 2));
                }
                auto light_r = agl::engine::Ray(position(v), light_p - position(v));
                auto lh = ray_trace(scene, light_r);
                if(lh) {
                    if(length(lh->hit.position - light_p) < 0.01) {
                        lightened = true;
                    }
                }
                auto lambertian = 0.f;
                if(lightened) {
                    auto light_dif = light_p - position(v);
                    auto d_to_l_2 = dot(light_dif);
                    lambertian = std::max(dot(normal(v), normalize(light_dif)), 0.f) * 30.f / d_to_l_2;
                }
                bool is_emissive = false;
                for(auto& ef : scene.emissive_faces) {
                    if(index(ef) == index(inter->triangle)) {
                        is_emissive = true;
                    }
                }
                if(is_emissive) {
                    lambertian = 1.f;
                }
                auto mid = material_id(ivert);
                auto& m = rasterization_scene.materials[mid];
                if(auto ptr = dynamic_cast<eng::Uniform<agl::Vec3>*>(m->uniforms["Kd"])) {
                    color(v) = lambertian * ptr->value;
                }
            }
        }
    }

    void update_render_passes() {
        clear(ambient_pass);
        clear(blinn_phong_pass);
        clear(wireframe_pass);

        auto wtc = agl::engine::world_to_clip(*camera);
        { // Update wireframe pass.
            // for(auto& o : rasterization_scene.objects) {
            //     auto& m = *o.cpu_mesh;
            //     auto bb = bounding_box(m);
            //     auto&& instance = subscribe(wireframe_pass, bounding_box_gizmo);
            //     assign_uniform(
            //         *instance,
            //         "world_to_clip",
            //         wtc * data::gizmo::bounding_box_model_to_world(bb));
            // }
            { // Frustrum gizmo.
                auto&& instance = subscribe(wireframe_pass, bounding_box_gizmo);
                instance->uniforms["rgb"]
                    = std::make_shared<eng::Uniform<agl::Vec3>>(agl::vec3(1.f, 1.f, 1.f));
                assign_uniform(
                    *instance,
                    "world_to_clip",
                    wtc * data::gizmo::bounding_box_model_to_world(
                        agl::engine::bounding_box(*frustrum_camera)));
            }
        }
        if(settings.frustrum_culling.enabled) { // Frustrum culling.
            statistics.frustrum_culling.accepted_count = 0;
            statistics.frustrum_culling.rejected_count = 0;
            auto fbb = agl::engine::bounding_box(*frustrum_camera);
            for(auto& o : rasterization_scene.objects) {
                auto& obb = o.bounding_box;
                if(are_intersecting(obb, fbb)) {
                    statistics.frustrum_culling.accepted_count += 1;
                    if(ambient_pass_loaded) {
                        subscribe(ambient_pass, o.gpu_mesh);
                    }
                    if(blinn_phong_pass_loaded) {
                        subscribe(blinn_phong_pass, o.gpu_mesh);
                    }
                } else {
                    statistics.frustrum_culling.rejected_count += 1;
                    if(settings.frustrum_culling.draw_rejected_bounding_boxes) {
                        auto&& instance = subscribe(wireframe_pass, bounding_box_gizmo);
                        assign_uniform(
                            *instance,
                            "world_to_clip",
                            wtc * data::gizmo::bounding_box_model_to_world(obb));
                        instance->uniforms["rgb"]
                        = std::make_shared<eng::Uniform<agl::Vec3>>(agl::vec3(1.f, 0.f, 0.f));
                    }
                    if(settings.frustrum_culling.draw_rejected_objects) {
                        if(ambient_pass_loaded) {
                            subscribe(ambient_pass, o.gpu_mesh);
                        }
                        if(blinn_phong_pass_loaded) {
                            subscribe(blinn_phong_pass, o.gpu_mesh);
                        }
                    }
                }
            }
        } else {
            statistics.frustrum_culling.accepted_count = 0;
            statistics.frustrum_culling.rejected_count = 0;
            for(auto& o : rasterization_scene.objects) {
                if(ambient_pass_loaded) {
                    subscribe(ambient_pass, o.gpu_mesh);
                }
                if(blinn_phong_pass_loaded) {
                    subscribe(blinn_phong_pass, o.gpu_mesh);
                }
            }
        }
        
    }

    void render() override {
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
            if constexpr(true) {
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
        

        if constexpr(true) { // Point pass.
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
};

void throwing_main() {
    auto p = GltfProgram();
    run(p);
}

int main() {
    try {
        throwing_main();
        return 0;
    } catch(const std::exception& e) {
        std::cerr << "std::exception: " << e.what() << std::endl;
    } catch(...) {
        std::cerr << "Unhandled exception." << std::endl;
    }
    return -1;
}
