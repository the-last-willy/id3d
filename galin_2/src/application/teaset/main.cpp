// Disable warnings.

#pragma warning(disable : 4005 4996)

// Local headers.

#include "nurbs/all.hpp"
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
        }
    }
    return cbm;
}

struct Object {
    std::shared_ptr<agl::engine::TriangleMesh> cpu_tessellated_mesh;

    std::shared_ptr<agl::engine::MeshInstance> gpu_control_mesh;
    std::shared_ptr<agl::engine::MeshInstance> gpu_tessellated_mesh;
};

struct App : Program {
    Settings settings;

    eng::ShaderCompiler shader_compiler;
    
    eng::Camera camera;

    agl::engine::RenderPass mesh_pass;
    agl::engine::RenderPass wireframe_pass;

    std::shared_ptr<eng::Mesh> sphere_gizmo;

    std::vector<Object> objects;

    std::shared_ptr<agl::engine::MeshInstance> transform_aoe;

    float time = 0.f;

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
        update_mesh();
    }

    void update(float dt) override {
        time += dt;
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

    void update_mesh() {
        objects.clear();
        { // Load mesh.
            auto cbm = load_teapot();
            for(auto& p : cbm.patches) {
                auto g = agl::common::Grid<agl::Vec3>(
                    agl::common::grid_indexing({4, 4}));
                for(uint32_t i = 0; i < 4; ++i)
                for(uint32_t j = 0; j < 4; ++j) {
                    at(g, i, j) = cbm.vertices[p[4 * i + j]];
                }
                auto& o = objects.emplace_back();
                o.cpu_tessellated_mesh = agl::standard::unique(sampled_mesh(
                    g, settings.tesselation_resolution, settings.tesselation_resolution));
                o.gpu_control_mesh = agl::standard::shared(
                    agl::engine::instance(agl::engine::wireframe(control_mesh(g))));
            }
        }
        { // Apply transformation.
            for(auto& o : objects) {
                for(auto& p : o.cpu_tessellated_mesh->geometry.vertex_positions) {
                    auto d = settings.transform_position - p;
                    auto l = length(d);
                    // d = -(settings.transform_radius - l) * normalize(d);
                    l = std::max(1.f - l / settings.transform_radius, 0.f);
                    // if(l > 0.f) {
                    //     l = 1.f - l;
                    // }
                    p += l * d * settings.transform_intensity;
                }
            }
        }
        { // To GPU.
            for(auto& o : objects) {
                o.gpu_tessellated_mesh = agl::standard::shared(
                    agl::engine::instance(agl::engine::triangle_mesh(*o.cpu_tessellated_mesh, {})));
            }
        }
    }

    void render() override {
        clear(agl::default_framebuffer, agl::depth_tag, 1.f);

        clear(mesh_pass);
        clear(wireframe_pass);

        auto wtc = agl::engine::world_to_clip(camera);
        auto wte = agl::engine::world_to_eye(camera);

        
        if(settings.show_triangulation) {
            for(auto& o : objects) {
                o.gpu_tessellated_mesh->uniforms["model_to_clip"]
                = std::make_shared<eng::Uniform<agl::Mat4>>(wtc);
                o.gpu_tessellated_mesh->uniforms["model_to_eye"]
                = std::make_shared<eng::Uniform<agl::Mat4>>(wte);
                subscribe(mesh_pass, o.gpu_tessellated_mesh);
            }
        }
        if(settings.show_control_points) {
            for(auto& o : objects) {
                o.gpu_control_mesh->uniforms["model_to_clip"]
                = std::make_shared<eng::Uniform<agl::Mat4>>(wtc);
                subscribe(wireframe_pass, o.gpu_control_mesh);
            }
        }
        if(settings.show_gizmo) {
            auto model_to_world = agl::translation(settings.transform_position)
            * agl::scaling3(settings.transform_radius);
            transform_aoe->uniforms["model_to_clip"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(wtc * model_to_world);
            subscribe(mesh_pass, transform_aoe);
        }
        
        agl::engine::render(mesh_pass);
        agl::engine::render(wireframe_pass);

        ui();
    }

    void ui() {
        if(ImGui::BeginMainMenuBar()) {
            ImGui::MenuItem("Settings", NULL, &settings.show_settings);
        }
        ImGui::EndMainMenuBar();
        if(ImGui::Begin("Settings")) {
            if(ImGui::Button("Reload")) {
                update_mesh();
            }
            if(ImGui::TreeNode("Debugging")) {
                ImGui::Checkbox("Show control points",
                    &settings.show_control_points);
                ImGui::Checkbox("Show gizmo",
                    &settings.show_gizmo);
                ImGui::Checkbox("Show triangulation",
                    &settings.show_triangulation);
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Tessellation")) {
                ImGui::SliderInt("Resolution",
                    &settings.tesselation_resolution, 2, 30);
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Transform")) {
                ImGui::SliderFloat("Intensity",
                    &settings.transform_intensity, 0.f, 1.f);
                ImGui::DragFloat3("Position", 
                    data(settings.transform_position), 0.05f);
                ImGui::SliderFloat("Radius",
                    &settings.transform_radius, 0.f, 5.f);
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
