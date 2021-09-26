// Disabled warnings.

#pragma warning(disable : 4005 4996)

// Local headers.

#include "data/all.hpp"
#include "mesh_conversion/all.hpp"
#include "all.hpp"

#include <local/all.hpp>
#include "program/all.hpp"

#include <agl/all.hpp>
#include <agl/format/off/all.hpp>

// External libraries.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <range/v3/view/indirect.hpp>
#include <range/v3/view/take.hpp>

// Standard library.

#include <iostream>

//

using namespace chaine;

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

    eng::ShaderCompiler shader_compiler = {};

    eng::Camera camera;

    eng::RenderPass edge_pass;
    eng::RenderPass edge_pass2;
    eng::RenderPass vertex_pass = {};

    std::vector<eng::RenderPass> triangle_passes = {};

    face_vertex::Mesh mesh;

    void refresh_mesh() {
        { // Edge pass.
            auto m = edges_mesh(mesh);
            for(auto& p : m->primitives) {
                p->material = std::make_shared<eng::Material>();
            }
            add(edge_pass, *m);
        }
        { // Edge pass 2.
            auto m = std::make_shared<eng::Mesh>(triangle_adjacency_mesh(mesh));
            for(auto& p : m->primitives) {
                p->material = std::make_shared<eng::Material>();
            }
            add(edge_pass2, *m);
        }
        { // Triangle pass.
            auto m = to_triangle_mesh(mesh);
            for(auto& p : m->primitives) {
                p->material = std::make_shared<eng::Material>();
            }
            for(auto& tp : triangle_passes) {
                add(tp, *m);
            }
        }
        { // Vertex pass.
            auto m = to_vertex_mesh(mesh);
            for(auto& p : m->primitives | ranges::views::indirect) {
                p.material = std::make_shared<eng::Material>();
            }
            add(vertex_pass, *m);
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

            triangle_passes.resize(3);
            triangle_passes[0].program = std::make_shared<eng::Program>(
                data::triangle_program(shader_compiler));
            triangle_passes[1].program = std::make_shared<eng::Program>(
                data::mean_curvature_program(shader_compiler));
            triangle_passes[2].program = std::make_shared<eng::Program>(
                data::smooth_normal_program(shader_compiler));
        }

        mesh = face_vertex::Mesh();
        {
            auto v0 = create_vertex(mesh);
            position(v0) = agl::vec3(0.f, 0.f, 0.f);
            
            auto v1 = create_vertex(mesh);
            position(v1) = agl::vec3(0.f, 1.f, 0.f);

            auto v2 = create_vertex(mesh);
            position(v2) = agl::vec3(1.f, 0.f, 0.f);

            auto t = create_triangle(mesh);
            topology(t).vertices[0] = v0;
            topology(t).vertices[1] = v1;
            topology(t).vertices[2] = v2;
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
                
                auto v = create_vertex(mesh);
                position(v) = (transform(camera) * agl::vec4(xpos, ypos, 0.f, 1.f)).xyz();

                auto t = create_triangle(mesh);
                topology(t).vertices[0] = face_vertex::VertexIndex(0);
                topology(t).vertices[1] = face_vertex::VertexIndex(1);
                topology(t).vertices[2] = v;

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
            auto& triangle_pass = triangle_passes[render_settings.selected_render_mode];
            bind(*triangle_pass.program);
            for(std::size_t i = 0; i < size(triangle_pass.primitives); ++i) {
                auto& p = *triangle_pass.primitives[i];
                auto& va = triangle_pass.vertex_arrays[i];
                bind(*p.material, *triangle_pass.program);
                bind(va);
                uniform(*triangle_pass.program, "mvp", agl::engine::view_to_camera_transform(camera));
                eng::render(p, va);
            }
            unbind(*triangle_pass.program);
        }
        
        if(render_settings.show_edges) {
            glLineWidth(render_settings.line_width);
            
            uniform(*edge_pass.program, "color", agl::vec3(1.f, 0.f, 0.f));
            bind(*edge_pass.program);
            for(std::size_t i = 0; i < size(edge_pass.primitives); ++i) {
                auto& p = *edge_pass.primitives[i];
                auto& va = edge_pass.vertex_arrays[i];
                bind(*p.material, *edge_pass.program);
                bind(va);
                uniform(*edge_pass.program, "mvp", agl::engine::view_to_camera_transform(camera));
                eng::render(p, va);
            }
            unbind(*edge_pass.program);
        }
        if(render_settings.show_edges) {
            glLineWidth(render_settings.line_width);
            
            uniform(*edge_pass2.program, "color", agl::vec3(0.f, 1.f, 0.f));
            bind(*edge_pass2.program);
            for(std::size_t i = 0; i < size(edge_pass2.primitives); ++i) {
                auto& p = *edge_pass2.primitives[i];
                auto& va = edge_pass2.vertex_arrays[i];
                bind(*p.material, *edge_pass2.program);
                bind(va);
                uniform(*edge_pass2.program, "mvp", agl::engine::view_to_camera_transform(camera));
                eng::render(p, va);
            }
            unbind(*edge_pass2.program);
        }
        if(render_settings.show_vertices) {
            bind(*vertex_pass.program);
            for(std::size_t i = 0; i < size(vertex_pass.primitives); ++i) {
                auto& p = *vertex_pass.primitives[i];
                auto& va = vertex_pass.vertex_arrays[i];
                bind(*p.material, *vertex_pass.program);
                bind(va);
                uniform(*vertex_pass.program, "mvp", agl::engine::view_to_camera_transform(camera));
                uniform(*vertex_pass.program, "point_size", render_settings.point_size);
                eng::render(p, va);
            }
            unbind(*vertex_pass.program);
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
