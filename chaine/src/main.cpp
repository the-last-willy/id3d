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
    bool show_edges = false;
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

        auto off = format::off::read("data/queen.off");

        auto mesh = triangle_mesh::Mesh();
        { // Off to triangle mesh.
            mesh.topology.triangles.resize(size(off.triangle_indices));
            for(std::size_t i = 0; i < size(off.triangle_indices); ++i) {
                auto& tis = off.triangle_indices[i];
                mesh.topology.triangles[i] = triangle_mesh::TriangleTopology{
                    triangle_mesh::VertexIndex{static_cast<uint32_t>(tis[0])},
                    triangle_mesh::VertexIndex{static_cast<uint32_t>(tis[1])},
                    triangle_mesh::VertexIndex{static_cast<uint32_t>(tis[2])}};
            }
            mesh.geometry.vertex_positions.resize(size(off.vertex_positions));
            for(std::size_t i = 0; i < size(off.vertex_positions); ++i) {
                mesh.geometry.vertex_positions[i] = off.vertex_positions[i];
            }
        }

        auto face_vertex_mesh = to_face_vertex_mesh(mesh);
        // for(auto v : vertices(face_vertex_mesh)) {
        //     color(v) = normalize(position(v)) * .5f + .5f;
        // }

        { // Compute Laplacian.
            for(auto v : vertices(face_vertex_mesh)) {
                auto vertex_area = 0.f;
                auto sum = agl::vec3(0.f);
                for(auto t : adjacent_triangles(v)) {
                    for(uint32_t i = 0; i < 3; ++i) {
                        if(index(vertex(t, i)) == index(v)) {
                            auto ui = position(vertex(t, i));
                            auto uj = position(vertex(t, (i + 1) % vertex_count(t)));
                            auto uk = position(vertex(t, (i + 2) % vertex_count(t)));
                            auto triangle_area = length(cross(ui - uk, uj - uk));
                            auto aij = dot(ui - uk, uj - uk) / triangle_area;
                            auto aik = dot(ui - uj, uk - uj) / triangle_area;
                            sum += aij * (uj - ui);
                            sum += aik * (uk - ui);
                        }
                    }
                    vertex_area += face_vertex::area(t);
                }
                vertex_area /= 3.f;
                auto laplacian = sum / (2.f * vertex_area);
                float mean_curvature = length(laplacian) / 2.f;
                color(v) = agl::vec3(1.f - 1.f / (1.f + mean_curvature / 20.f));
                normal(v) = -normalize(laplacian);
            }
        }
        { // Flip incorrect normals.
            for(auto&& t : triangles(face_vertex_mesh)) {
                auto v0 = vertex(t, 0);
                auto v1 = vertex(t, 1);
                auto v2 = vertex(t, 2);
                auto tn = normalize(cross(
                    position(v1) - position(v0),
                    position(v2) - position(v0)));
                if(dot(normal(v0), tn) < 0.f) {
                    normal(v0) = -normal(v0);
                }
                if(dot(normal(v1), tn) < 0.f) {
                    normal(v1) = -normal(v1);
                }
                if(dot(normal(v2), tn) < 0.f) {
                    normal(v2) = -normal(v2);
                }
            }
        }

        { // Edge pass.
            auto m = triangle_mesh::edge_mesh(mesh);
            for(auto& p : m->primitives) {
                p->material = std::make_shared<eng::Material>();
            }
            add(edge_pass, *m);
        }
        { // Edge pass 2.
            auto m = std::make_shared<eng::Mesh>(triangle_adjacency_mesh(face_vertex_mesh));
            for(auto& p : m->primitives) {
                p->material = std::make_shared<eng::Material>();
            }
            add(edge_pass2, *m);
        }
        { // Triangle pass.
            auto m = to_triangle_mesh(face_vertex_mesh);
            for(auto& p : m->primitives) {
                p->material = std::make_shared<eng::Material>();
            }
            for(auto& tp : triangle_passes) {
                add(tp, *m);
            }
        }
        { // Vertex pass.
            auto m = to_vertex_mesh(face_vertex_mesh);
            for(auto& p : m->primitives | ranges::views::indirect) {
                p.material = std::make_shared<eng::Material>();
            }
            add(vertex_pass, *m);
        }

        { // Camera.
            if(auto pp = std::get_if<eng::PerspectiveProjection>(&camera.projection)) {
                pp->aspect_ratio = 16.f / 9.f;
            }
        }
    }

    void update(float) override {
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
        {
            if(glfwGetKey(window.window, GLFW_KEY_A)) {
                auto direction = (rotation(camera.view) * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
                camera.view.position = camera.view.position - direction / 100.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_D)) {
                auto direction = (rotation(camera.view) * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
                camera.view.position = camera.view.position + direction / 100.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_S)) {
                auto direction = rotation(camera.view)[2].xyz();
                camera.view.position = camera.view.position + direction / 100.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_W)) {
                auto direction = rotation(camera.view)[2].xyz();
                camera.view.position = camera.view.position - direction / 100.f;
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
            
            uniform(*edge_pass2.program, "color", agl::vec3(1.f, 0.f, 0.f));
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
