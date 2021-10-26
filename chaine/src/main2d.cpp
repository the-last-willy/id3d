// Disabled warnings.

#pragma warning(disable : 4005 4996)

// Local headers.

#include "geometry/all.hpp"
#include "topology/all.hpp"

#include "data/all.hpp"
#include "mesh_conversion/all.hpp"
#include "all.hpp"

#include <local/all.hpp>
#include "program/all.hpp"

#include <agl/opengl/all.hpp>
#include <agl/format/off/all.hpp>

// External libraries.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <range/v3/view/indirect.hpp>
#include <range/v3/view/take.hpp>

// Standard library.

#include <iostream>
#include <random>

//

using namespace chaine;

inline
auto create_random_generator() {
    auto rd = std::random_device();
    return std::mt19937(rd());
}

struct RenderSettings {
    bool show_edges = true;
    bool show_triangles = true;
    bool show_vertices = false;

    int selected_render_mode = 0;

    float line_width = 1.f;
    float point_size = 1.f;
};

struct App : Program {
    RenderSettings render_settings;

    std::mt19937 random_generator = create_random_generator();

    eng::ShaderCompiler shader_compiler = {};

    eng::Camera camera;

    agl::engine::RenderPass edge_pass;
    agl::engine::RenderPass edge_pass2;
    agl::engine::RenderPass vertex_pass = {};

    agl::engine::RenderPass triangle_pass = {};

    face_vertex::Mesh mesh;

    void refresh_mesh() {
        { // Edge pass.
            auto m = std::make_shared<eng::Mesh>(edges_mesh(mesh));
            edge_pass.subscriptions.clear();
            subscribe(edge_pass, m);
        }
        { // Edge pass 2.
            auto m = std::make_shared<eng::Mesh>(triangle_adjacency_mesh(mesh));
            edge_pass2.subscriptions.clear();
            subscribe(edge_pass2, m);
        }
        { // Triangle pass.
            auto m = std::make_shared<eng::Mesh>(triangles_mesh(mesh));
            subscribe(triangle_pass, m);
        }
        { // Vertex pass.
            auto m = std::make_shared<eng::Mesh>(vertices_mesh(mesh));
            subscribe(vertex_pass, m);
        }
    }

    void init() override {
        shader_compiler.log_folder = "logs/";
        shader_compiler.root = "chaine/src/";

        { // Render passes.
            edge_pass.program = std::make_shared<eng::Program>(
                data::edge_program(shader_compiler));
            edge_pass2.program = std::make_shared<eng::Program>(
                data::edge_program(shader_compiler));
            vertex_pass.program = std::make_shared<eng::Program>(
                data::vertex_program(shader_compiler));
            triangle_pass.program = std::make_shared<eng::Program>(
                data::triangle_program(shader_compiler));
        }

        mesh = face_vertex::Mesh();
        {
            auto v0 = create_vertex(mesh);
            position(v0) = agl::vec3(-1.f, -1.f, 0.f);
            auto v1 = create_vertex(mesh);
            position(v1) = agl::vec3(1.f, -1.f, 0.f);
            auto v2 = create_vertex(mesh);
            position(v2) = agl::vec3(-1.f, 1.f, 0.f);
            auto t = create_triangle(mesh);
            topology(t)->vertices[0] = v0;
            topology(t)->vertices[1] = v1;
            topology(t)->vertices[2] = v2;
            topology(v0)->triangle = t;
            topology(v1)->triangle = t;
            topology(v2)->triangle = t;
        }

        refresh_mesh();

        { // Camera.
            auto op = agl::engine::OrthographicProjection();
            op.z_far = 10.f;
            op.z_near = -10.f;
            camera.projection = std::move(op);
        }
    }

    void update(float) override {
        {
            if(glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_2)) {
                double xpos, ypos;
                glfwGetCursorPos(window.window, &xpos, &ypos);
                xpos = 2.f * xpos / window.width() - 1.f;
                ypos = -2.f * ypos / window.height() + 1.f;
                
                auto pos = (transform(camera) * agl::vec4(float(xpos), float(ypos), 0.f, 1.f)).xyz();

                insert_vertex(mesh, pos);

                refresh_mesh();
            }
        }
        {
            if(glfwGetKey(window.window, GLFW_KEY_A)) {
                auto direction = (rotation(camera.view) * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
                camera.view.position = camera.view.position - direction / 50.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_D)) {
                auto direction = (rotation(camera.view) * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
                camera.view.position = camera.view.position + direction / 50.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_S)) {
                auto direction = (rotation(camera.view) * agl::rotation_x(agl::constant::pi / 2.f))[2].xyz();
                camera.view.position = camera.view.position + direction / 50.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_W)) {
                auto direction = (rotation(camera.view) * agl::rotation_x(agl::constant::pi / 2.f))[2].xyz();
                camera.view.position = camera.view.position - direction / 50.f;
            }
        }
    }

    void render() override {
        glClearDepthf(1.f);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        if(render_settings.show_triangles) {
            uniform(*triangle_pass.program, "mvp", agl::engine::world_to_clip(camera));
            agl::engine::render(triangle_pass);
        }
        
        if(render_settings.show_edges) {
            glLineWidth(render_settings.line_width);
            
            uniform(*edge_pass.program, "color", agl::vec3(1.f, 0.f, 0.f));
            uniform(*edge_pass.program, "mvp", agl::engine::world_to_clip(camera));
            agl::engine::render(edge_pass);
        }
        if(render_settings.show_edges) {
            glLineWidth(render_settings.line_width);
            
            uniform(*edge_pass2.program, "color", agl::vec3(0.f, 1.f, 0.f));
            uniform(*edge_pass2.program, "mvp", agl::engine::world_to_clip(camera));
            agl::engine::render(edge_pass2);
        }
        if(render_settings.show_vertices) {
            uniform(*vertex_pass.program, "mvp", agl::engine::world_to_clip(camera));
            uniform(*vertex_pass.program, "point_size", render_settings.point_size);
            agl::engine::render(edge_pass2);
        }
        { // UI
            ImGui::Begin("Settings");

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

            if(ImGui::Button("Collapse random edge")) {
                while(true) {
                    auto&& rte = random_triangle_edge(mesh, random_generator);
                    if(can_collapse(rte)) {
                        collapse(rte);
                        break;
                    }
                }
                refresh_mesh();
            }

            ImGui::End();
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

