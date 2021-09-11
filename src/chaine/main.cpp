// Definitions.

#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

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

    float point_size = 1.f;
};

struct App : Program {
    RenderSettings render_settings;

    eng::ShaderCompiler shader_compiler = {};

    tlw::View view = {};
    eng::PerspectiveProjection projection = {};

    eng::RenderPass edge_pass;
    eng::RenderPass edge_pass2;
    eng::RenderPass triangle_pass = {};
    eng::RenderPass vertex_pass = {};

    void init() override {
        shader_compiler.root = local::src_folder;

        { // Render passes.
            edge_pass.program = std::make_shared<eng::Program>(
                data::edge_program(shader_compiler));
            edge_pass2.program = std::make_shared<eng::Program>(
                data::edge_program(shader_compiler));
            triangle_pass.program = std::make_shared<eng::Program>(
                data::triangle_program(shader_compiler));
            vertex_pass.program = std::make_shared<eng::Program>(
                data::vertex_program(shader_compiler));
        }

        auto off = format::off::read(local::root_folder + "/data/queen.off");

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
                            auto aij = dot(ui - uk, uj - uk) / length(cross(ui - uk, uj - uk));
                            auto aik = dot(ui - uj, uk - uj) / length(cross(ui - uj, uk - uj));
                            sum += aij * (uj - ui);
                            sum += aik * (uk - ui);
                        }
                    }
                    vertex_area += face_vertex_mesh::area(t);
                }
                vertex_area /= 3.f;
                auto laplacian = sum / (2.f * vertex_area);
                normal(v) = -normalize(laplacian);
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
            auto m = std::make_shared<eng::Mesh>(vertex_adjacency_mesh(face_vertex_mesh));
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
            add(triangle_pass, *m);
        }
        { // Vertex pass.
            auto m = to_vertex_mesh(face_vertex_mesh);
            for(auto& p : m->primitives | ranges::views::indirect) {
                p.material = std::make_shared<eng::Material>();
            }
            add(vertex_pass, *m);
        }

        projection.aspect_ratio = 16.f / 9.f;
        projection.z_near = 0.01f;
        projection.z_far = 100.f;
    }

    void update(float) override {
        if(glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_1)) {
            glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            agl::Vec2 d = current_cursor_pos - previous_cursor_pos;
            view.yaw += d[0] / 500.f;
            view.pitch += d[1] / 500.f;

            previous_cursor_pos = current_cursor_pos;
        } else {
            glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            previous_cursor_pos = current_cursor_pos;
        }
        {
            if(glfwGetKey(window.window, GLFW_KEY_A)) {
                auto direction = (rotation(view) * agl::rotation_y(agl::pi / 2.f))[2].xyz();
                view.position = view.position - direction / 100.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_D)) {
                auto direction = (rotation(view) * agl::rotation_y(agl::pi / 2.f))[2].xyz();
                view.position = view.position + direction / 100.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_S)) {
                auto direction = rotation(view)[2].xyz();
                view.position = view.position - direction / 100.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_W)) {
                auto direction = rotation(view)[2].xyz();
                view.position = view.position + direction / 100.f;
            }
        }
    }

    void render() override {
        glClearDepthf(1.f);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        if(render_settings.show_triangles) {
            bind(*triangle_pass.program);
            for(std::size_t i = 0; i < size(triangle_pass.primitives); ++i) {
                auto& p = *triangle_pass.primitives[i];
                auto& va = triangle_pass.vertex_arrays[i];
                bind(*p.material, *triangle_pass.program);
                bind(va);
                uniform(*triangle_pass.program, "mvp", transform(projection) * inverse(transform(view)));
                eng::render(p, va);
            }
            unbind(*triangle_pass.program);
        }
        
        if(render_settings.show_edges) {
            glLineWidth(5.f);
            
            uniform(*edge_pass.program, "color", agl::vec3(1.f, 0.f, 0.f));
            bind(*edge_pass.program);
            for(std::size_t i = 0; i < size(edge_pass.primitives); ++i) {
                auto& p = *edge_pass.primitives[i];
                auto& va = edge_pass.vertex_arrays[i];
                bind(*p.material, *edge_pass.program);
                bind(va);
                uniform(*edge_pass.program, "mvp", transform(projection) * inverse(transform(view)));
                eng::render(p, va);
            }
            unbind(*edge_pass.program);

            { // Edge pass 2.
                uniform(*edge_pass2.program, "color", agl::vec3(0.f, 0.f, 1.f));
                bind(*edge_pass2.program);
                for(std::size_t i = 0; i < size(edge_pass2.primitives); ++i) {
                    auto& p = *edge_pass2.primitives[i];
                    auto& va = edge_pass2.vertex_arrays[i];
                    bind(*p.material, *edge_pass2.program);
                    bind(va);
                    uniform(*edge_pass2.program, "mvp", transform(projection) * inverse(transform(view)));
                    eng::render(p, va);
                }
                unbind(*edge_pass2.program);
            }
        }
        if(render_settings.show_vertices) {
            bind(*vertex_pass.program);
            for(std::size_t i = 0; i < size(vertex_pass.primitives); ++i) {
                auto& p = *vertex_pass.primitives[i];
                auto& va = vertex_pass.vertex_arrays[i];
                bind(*p.material, *vertex_pass.program);
                bind(va);
                uniform(*vertex_pass.program, "mvp", transform(projection) * inverse(transform(view)));
                uniform(*vertex_pass.program, "point_size", render_settings.point_size);
                eng::render(p, va);
            }
            unbind(*vertex_pass.program);
        }
        { // UI
            ImGui::Begin("Settings");
            ImGui::Checkbox("Show vertices", &render_settings.show_vertices);
            ImGui::Checkbox("Show edges", &render_settings.show_edges);
            ImGui::Checkbox("Show triangles", &render_settings.show_triangles);
            ImGui::End();

            ImGui::Begin("Point size");
            ImGui::SliderFloat("float", &render_settings.point_size, 1.0f, 10.0f);
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
