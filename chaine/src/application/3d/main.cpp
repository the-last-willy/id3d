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
    std::random_device rd;
    return std::mt19937(rd());
}

struct RenderSettings {
    bool show_edges = false;
    bool show_triangles = true;
    bool show_vertices = false;

    int selected_render_mode = 0;

    float line_width = 1.f;
    float point_size = 1.f;

    float speed = 3.f / 100.f;
};

struct App : Program {
    RenderSettings render_settings;

    std::mt19937 random_generator = create_random_generator();

    eng::ShaderCompiler shader_compiler;

    eng::Camera camera;

    agl::engine::RenderPass edge_pass;
    agl::engine::RenderPass edge_pass2;
    agl::engine::RenderPass vertex_pass;

    std::array<agl::engine::RenderPass, 3> triangle_passes;

    face_vertex::Mesh face_vertex_mesh;

    void refresh_mesh() {
        { // Edge pass.
            auto m = std::make_shared<eng::Mesh>(edges_mesh(face_vertex_mesh));
            edge_pass.subscriptions.clear();
            subscribe(edge_pass, m);
        }
        { // Edge pass 2.
            auto m = std::make_shared<eng::Mesh>(triangle_adjacency_mesh(face_vertex_mesh));
            edge_pass2.subscriptions.clear();
            subscribe(edge_pass2, m);
        }
        { // Triangle pass.
            auto m = std::make_shared<eng::Mesh>(triangles_mesh(face_vertex_mesh));
            for(auto& tp : triangle_passes) {
                tp.subscriptions.clear();
                subscribe(tp, m);
            }
        }
        { // Vertex pass.
            auto m = std::make_shared<eng::Mesh>(vertices_mesh(face_vertex_mesh));
            vertex_pass.subscriptions.clear();
            subscribe(vertex_pass, m);
        }

        { // Camera.
            if(auto pp = std::get_if<eng::PerspectiveProjection>(&camera.projection)) {
                pp->aspect_ratio = 16.f / 9.f;
                pp->z_far = 10.f;
                pp->z_near = 0.01f;
            }
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

            triangle_passes[0].program = std::make_shared<eng::Program>(
                data::triangle_program(shader_compiler));
            triangle_passes[1].program = std::make_shared<eng::Program>(
                data::mean_curvature_program(shader_compiler));
            triangle_passes[2].program = std::make_shared<eng::Program>(
                data::smooth_normal_program(shader_compiler));
        }

        auto off = format::off::read("chaine/data/queen.off");

        triangle_mesh::Mesh mesh;
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

        face_vertex_mesh = to_face_vertex_mesh(mesh);
        
        // IDK WHAT I WAS TRYING TO DO HERE.
        // for(uint32_t i = 0; i < 100; ++i) {
        //     auto tp = proxy(
        //         face_vertex_mesh,
        //         face_vertex::TriangleIndex(i));
        //     auto vep = proxy(
        //         face_vertex_mesh,
        //         face_vertex::TriangleEdgeIndex(std::array{
        //             index(tp), 
        //             index(adjacent_triangle(tp, 0))}));
        // }

        // { // Collapse some random edges.
        //     auto rte = random_triangle_edge(face_vertex_mesh, random_generator);
        //     collapse(rte);
        // }

        // { // Compute Laplacian.
        //     for(auto v : vertices(face_vertex_mesh)) {
        //         auto vertex_area = 0.f;
        //         auto sum = agl::vec3(0.f);
        //         for(auto t : adjacent_triangles(v)) {
        //             for(uint32_t i = 0; i < 3; ++i) {
        //                 if(index(vertex(t, i)) == index(v)) {
        //                     auto ui = position(vertex(t, i));
        //                     auto uj = position(vertex(t, (i + 1) % vertex_count(t)));
        //                     auto uk = position(vertex(t, (i + 2) % vertex_count(t)));
        //                     auto triangle_area = length(cross(ui - uk, uj - uk));
        //                     auto aij = dot(ui - uk, uj - uk) / triangle_area;
        //                     auto aik = dot(ui - uj, uk - uj) / triangle_area;
        //                     sum += aij * (uj - ui);
        //                     sum += aik * (uk - ui);
        //                 }
        //             }
        //             vertex_area += face_vertex::area(t);
        //         }
        //         vertex_area /= 3.f;
        //         auto laplacian = sum / (2.f * vertex_area);
        //         float mean_curvature = length(laplacian) / 2.f;
        //         color(v) = agl::vec3(1.f - 1.f / (1.f + mean_curvature / 20.f));
        //         normal(v) = -normalize(laplacian);
        //     }
        // }
        // { // Flip incorrect normals.
        //     for(auto&& t : triangles(face_vertex_mesh)) {
        //         auto v0 = vertex(t, 0);
        //         auto v1 = vertex(t, 1);
        //         auto v2 = vertex(t, 2);
        //         auto tn = normalize(cross(
        //             position(v1) - position(v0),
        //             position(v2) - position(v0)));
        //         if(dot(normal(v0), tn) < 0.f) {
        //             normal(v0) = -normal(v0);
        //         }
        //         if(dot(normal(v1), tn) < 0.f) {
        //             normal(v1) = -normal(v1);
        //         }
        //         if(dot(normal(v2), tn) < 0.f) {
        //             normal(v2) = -normal(v2);
        //         }
        //     }
        // }

        refresh_mesh();
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
                camera.view.position = camera.view.position - direction * render_settings.speed;
            }
            if(glfwGetKey(window.window, GLFW_KEY_D)) {
                auto direction = (rotation(camera.view) * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
                camera.view.position = camera.view.position + direction * render_settings.speed;
            }
            if(glfwGetKey(window.window, GLFW_KEY_S)) {
                auto direction = rotation(camera.view)[2].xyz();
                camera.view.position = camera.view.position + direction * render_settings.speed;
            }
            if(glfwGetKey(window.window, GLFW_KEY_W)) {
                auto direction = rotation(camera.view)[2].xyz();
                camera.view.position = camera.view.position - direction * render_settings.speed;
            }
        }
    }

    void render() override {
        glClearDepthf(1.f);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        if(render_settings.show_triangles) {
            auto& triangle_pass = triangle_passes[render_settings.selected_render_mode];
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
            agl::engine::render(vertex_pass);
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
                std::cout << vertex_count(face_vertex_mesh) << std::endl;
                for(int i = 0; i < 1000; ++i) {
                // while(vertex_count(face_vertex_mesh) > 1000) {
                    // std::cout << vertex_count(face_vertex_mesh) << std::endl;
                    int j;
                    for(j = 0; j < 100; ++j) {
                        auto&& rte = random_triangle_edge(face_vertex_mesh, random_generator);
                        if(can_collapse(rte)) {
                            auto t0 = adjacent_triangle(rte, 0);
                            auto t1 = adjacent_triangle(rte, 1);
                            auto va = vertex_after(t0, opposite_vertex(t0, t1));
                            auto vb = vertex_after(t1, opposite_vertex(t1, t0));
                            auto&& v = collapse(rte);
                            position(v) = (position(va) + position(vb)) / 2.f;
                            break;
                        }
                    }
                    if(j == 100) {
                        std::cout << "STOP" << std::endl;
                        break;
                    }
                    refresh_mesh();
                }
                
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
