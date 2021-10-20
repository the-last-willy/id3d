// Definitions.

#define GLFW_INCLUDE_NONE
#define TINYGLTF_IMPLEMENTATION

// Disabled warnings.

#pragma warning(disable : 4005 4996)

// Local headers.

#include "settings.hpp"

#include "data/all.hpp"
#include "nurbs/all.hpp"

#include "error_callback.hpp"

#include <program/all.hpp>

// External libraries.

#include <agl/engine/all.hpp>
#include <agl/format/wavefront/all.hpp>
#include <agl/standard/all.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard library.

#include <iostream>

//

struct App : Program {
    Settings settings;

    eng::ShaderCompiler shader_compiler;
    
    eng::Camera camera;

    agl::engine::RenderPass mesh_pass;
    agl::engine::RenderPass wireframe_pass;

    std::shared_ptr<eng::Mesh> wireframe;

    agl::format::wavefront::Content database;

    float time = 0.f;

    void load_mesh() {
        
    }

    void init() override {
        { // Shader compiler.
            shader_compiler.log_folder = "logs/";
            shader_compiler.root = "galin/src/shader";
        }
        { // Render passes.
            mesh_pass.program = std::make_shared<eng::Program>(
                data::flat_shading_program(shader_compiler));
            wireframe_pass.program = std::make_shared<eng::Program>(
                data::wireframe_program(shader_compiler));
        }
        { // Load mesh.
            database = agl::format::wavefront::load(
                "D:/data/cornell-box/cornell-box.obj",
                "D:/data/cornell-box");
                // "C:/Users/Willy/Desktop/data/wavefront/CornellBox/cornell-box.obj",
                // "C:/Users/Willy/Desktop/data/wavefront/CornellBox");
            database.meshes.clear();
            for(auto&& tm : database.tmeshes) {
                database.meshes.push_back(std::make_shared<eng::Mesh>(
                    agl::engine::render_mesh(*tm, database.materials)));
            }

            // wireframe = std::make_shared<eng::Mesh>(
            //     agl::engine::wireframe(*database.tmeshes.front()));
        }
        { // 
            for(auto&& m : database.meshes) {
                subscribe(mesh_pass, m);
            }
            // subscribe(wireframe_pass, wireframe);
        }
        { // Camera.
            if(auto pp = std::get_if<eng::PerspectiveProjection>(&camera.projection)) {
                pp->aspect_ratio = 16.f / 9.f;
            }
        }
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

    void ui() {
        
    }

    void render() override {
        clear(agl::default_framebuffer, agl::depth_tag, 1.f);

        auto wtc = agl::engine::world_to_clip(camera);
        auto wte = agl::engine::world_to_eye(camera);
        if(settings.show_mesh) {
            mesh_pass.uniforms["world_to_clip"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(wtc);
            mesh_pass.uniforms["world_to_eye"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(wte);
            agl::engine::render(mesh_pass);
        }
        if(settings.show_wireframe) {
            wireframe_pass.uniforms["world_to_clip"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(wtc);
            agl::engine::render(wireframe_pass);
        }
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
