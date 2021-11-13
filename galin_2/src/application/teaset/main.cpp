// Disable warnings.

#pragma warning(disable : 4005 4996)

// Local headers.

#include "modelling/all.hpp"
#include "settings.hpp"

#include <agl/engine/all.hpp>
#include <agl/standard/all.hpp>
#include <id3d/common/all.hpp>

// External libraries.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard library.

#include <fstream>
#include <iostream>
#include <random>

//

struct CubicBezierMesh {
    std::vector<std::array<uint32_t, 16>> patches;
    std::vector<agl::Vec3> vertices;
};

CubicBezierMesh load_teapot() {
    auto file = std::ifstream("galin_2/data/teaset/teapot");
    if(not file.is_open()) {
        throw std::runtime_error("Failed to open teapot.");
    }
    auto cbm = CubicBezierMesh();
    { // Patches.
        uint32_t patch_count;
        file >> patch_count;
        cbm.patches.resize(patch_count);
        for(auto& p : cbm.patches) {
            file >> p[0];
            p[0] -= 1;
            for(uint32_t i = 1; i < 16; ++i) {
                file.ignore(1); // ','
                file >> p[i];
                p[i] -= 1;
            }
        }
    }
    { // Vertices.
        uint32_t vertex_count;
        file >> vertex_count;
        cbm.vertices.resize(vertex_count);
        for(auto& v : cbm.vertices) {
            file >> v[0];
            for(uint32_t i = 1; i < 3; ++i) {
                file.ignore(1); // ','
                file >> v[i];
            }
            std::swap(v[1], v[2]);
        }
    }
    return cbm;
}

struct Object2 {
    std::shared_ptr<agl::engine::TriangleMesh> cpu_tessellated_mesh;

    std::shared_ptr<agl::engine::MeshInstance> gpu_control_mesh;
    std::shared_ptr<agl::engine::MeshInstance> gpu_tessellated_mesh;
};

struct Scene {
    agl::engine::BoundingBox cpu_bounding_box;
    std::shared_ptr<agl::engine::MeshInstance> gpu_bounding_box;
};

struct App : Program {
    std::default_random_engine random_generator
    = std::default_random_engine(agl::standard::random_seed());

    Settings settings;

    eng::ShaderCompiler shader_compiler;
    
    eng::Camera camera;

    agl::engine::RenderPass mesh_pass;
    agl::engine::RenderPass blend_pass;
    agl::engine::RenderPass wireframe_pass;

    std::shared_ptr<eng::Mesh> sphere_gizmo;

    std::shared_ptr<agl::engine::MeshInstance> transform_aoe;

    Object object;

    std::optional<FFD> ffd;
    std::shared_ptr<agl::engine::MeshInstance> ffd_control_mesh;

    void init() override {
        { // Shader compiler.
            shader_compiler.log_folder = "logs/";
        }
        { // Render passes.
            assign_program(mesh_pass,
                shader::flat_shading(shader_compiler));
            assign_program(wireframe_pass,
                shader::wireframe(shader_compiler));
        }
        { // Sphere gizmo.
            sphere_gizmo = std::make_shared<eng::Mesh>(
                agl::engine::triangle_mesh(gizmo::uv_sphere(), {}));
            transform_aoe = subscribe(mesh_pass, sphere_gizmo);
        }
        { // Camera.
            if(auto pp = std::get_if<eng::PerspectiveProjection>(&camera.projection)) {
                pp->aspect_ratio = 16.f / 9.f;
            }
        }
        update_object();
    }

    void update(float) override {
        if(not ImGui::GetIO().WantCaptureMouse) {
            if(glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_1)) {
                glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                agl::Vec2 d = current_cursor_pos - previous_cursor_pos;
                camera.view.yaw -= d[0] / 500.f;
                camera.view.pitch -= d[1] / 500.f;

                previous_cursor_pos = current_cursor_pos;
            } else {
                glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                previous_cursor_pos = current_cursor_pos;
            }
        }
        {
            if(glfwGetKey(window.window, GLFW_KEY_A)) {
                auto direction = (rotation(camera.view) * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
                camera.view.position = camera.view.position - direction / 10.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_D)) {
                auto direction = (rotation(camera.view) * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
                camera.view.position = camera.view.position + direction / 10.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_S)) {
                auto direction = rotation(camera.view)[2].xyz();
                camera.view.position = camera.view.position + direction / 10.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_W)) {
                auto direction = rotation(camera.view)[2].xyz();
                camera.view.position = camera.view.position - direction / 10.f;
            }
        }
    }

    void update_object() {
        { // Load patches.
            auto cbm = load_teapot();
            object.patches.resize(size(cbm.patches));
            for(std::size_t h = 0; h < size(cbm.patches); ++h) {
                auto& p = cbm.patches[h];
                auto g = agl::common::Grid<agl::Vec3>(
                    agl::common::grid_indexing({4, 4}));
                for(uint32_t i = 0; i < 4; ++i)
                for(uint32_t j = 0; j < 4; ++j) {
                    at(g, i, j) = cbm.vertices[p[4 * i + j]];
                }
                auto& pa = object.patches[h];
                pa.control_points = std::move(g);
                update_control_mesh(pa);
            }
        }
        { // Tesselation.
            tesselate(object, std::size_t(settings.tesselation_resolution));
        }
        { // Transform.
            if(settings.transform_enabled) {
                auto attenuation = attenuation_function(
                    TransformAttenuation(settings.transform_attenuation));
                auto transformation = std::function<agl::Vec3(agl::Vec3)>();
                transform(object, [&](agl::Vec3 p) {
                    auto d = settings.transform_position - p;
                    auto a = attenuation((settings.transform_position - p) / settings.transform_radius);
                    return p + a * d * settings.transform_intensity;
                });
            }
        }
        { // FFD.
            if(settings.ffd_enabled) {
                update_bounding_box(object);
                auto norm_a = 1.f / length(object.bounding_box);
                auto norm_b = -lower_bound(object.bounding_box) * norm_a;
                transform(object, [&](agl::Vec3 p) {
                    auto uvw = norm_a * p + norm_b;
                    return bezier(ffd->control_points, uvw[0], uvw[1], uvw[2]);
                });
            }
        }
        { // GPU.
            update_bounding_box(object);
            update_gpu(object);
        }
    }

    void render() override {
        clear(agl::default_framebuffer, agl::depth_tag, 1.f);

        clear(mesh_pass);
        clear(wireframe_pass);

        auto wtc = agl::engine::world_to_clip(camera);
        auto wte = agl::engine::world_to_eye(camera);

        
        if(settings.show_triangulation) {
            for(auto& p : object.patches) {
                p.gpu_tesselation->uniforms["model_to_clip"]
                = std::make_shared<eng::Uniform<agl::Mat4>>(wtc);
                p.gpu_tesselation->uniforms["model_to_eye"]
                = std::make_shared<eng::Uniform<agl::Mat4>>(wte);
                subscribe(mesh_pass, p.gpu_tesselation);
            }
        }
        if(settings.show_control_points) {
            for(auto& p : object.patches) {
                p.gpu_control_mesh->uniforms["model_to_clip"]
                = std::make_shared<eng::Uniform<agl::Mat4>>(wtc);
                subscribe(wireframe_pass, p.gpu_control_mesh);
            }
        }
        if(settings.show_gizmo) {
            auto model_to_world = agl::translation(settings.transform_position)
            * agl::scaling3(settings.transform_radius);
            transform_aoe->uniforms["model_to_clip"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(wtc * model_to_world);
            subscribe(mesh_pass, transform_aoe);
        }
        if(settings.show_bounding_box) {
            object.gpu_bounding_box->uniforms["model_to_clip"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(wtc
                * gizmo::box_wireframe_model_to_world(object.bounding_box));
            subscribe(wireframe_pass, object.gpu_bounding_box);
        }
        { // FFD.
            if(settings.ffd_show_control_grid) {
                if(ffd_control_mesh) {
                    ffd_control_mesh->uniforms["model_to_clip"]
                    = std::make_shared<eng::Uniform<agl::Mat4>>(wtc);
                    subscribe(wireframe_pass, ffd_control_mesh);
                }
            }
        }
        agl::engine::render(mesh_pass);
        agl::engine::render(wireframe_pass);

        ui();
    }

    void generate_ffd() {

        ffd = ::ffd(
            {
                std::size_t(settings.ffd.resolution[0]),
                std::size_t(settings.ffd.resolution[1]),
                std::size_t(settings.ffd.resolution[2])
            },
            object.bounding_box);
        { // Control mesh.
            ffd_control_mesh = agl::standard::shared(
                agl::engine::instance(
                    agl::engine::wireframe(
                        control_mesh3(ffd->control_points))));
        }
    }

    void ui() {
        if(ImGui::BeginMainMenuBar()) {
            ImGui::MenuItem("Settings", NULL, &settings.show_settings);
        }
        ImGui::EndMainMenuBar();
        if(ImGui::Begin("Settings")) {
            if(ImGui::Button("Reload")) {
                update_object();
            }
            if(ImGui::TreeNode("File")) {
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("View")) {
                ImGui::Checkbox("Show bounding box",
                    &settings.show_bounding_box);
                ImGui::Checkbox("Show control points",
                    &settings.show_control_points);
                
                ImGui::Checkbox("Show triangulation",
                    &settings.show_triangulation);
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Scene")) {
                if(ImGui::TreeNode("Patchs")) {
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
                    for(std::size_t i = 0; i < size(object.patches); ++i) {
                        auto& patch = object.patches[i];
                        if(ImGui::TreeNode(("#" + std::to_string(i)).c_str())) {
                            if(ImGui::SliderFloat3(("Color##" + std::to_string(i)).c_str(), data(patch.color), 0.f, 1.f)) {
                                patch.gpu_tesselation->uniforms["color_factor"]
                                = std::make_shared<eng::Uniform<agl::Vec4>>(patch.color);
                            }
                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Tessellation")) {
                ImGui::SliderInt("Resolution",
                    &settings.tesselation_resolution, 2, 30);
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Transform")) {
                ImGui::Checkbox("Enabled", &settings.transform_enabled);
                ImGui::NewLine();

                ImGui::Combo("Area of effect", &settings.transform_attenuation, "none\0linear\0cubic\0\0");
                ImGui::Combo("Type", &settings.transform_type, "none\0scaling\0twist\0\0");
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

                ImGui::TreePop();
            }
        }
        ImGui::End();
    }
};

void throwing_main() {
    auto a = App();
    run(a);
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
