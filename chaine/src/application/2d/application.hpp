#include "data/all.hpp"
#include "geometry/all.hpp"
#include "mesh_conversion/all.hpp"
#include "ruppert/all.hpp"
#include "topology/all.hpp"
#include "all.hpp"
#include "insert.hpp"
#include "is_delaunay.hpp"
#include "lawson.hpp"
#include "paraboloid.hpp"
#include "settings.hpp"

#include <agl/opengl/all.hpp>
#include <agl/format/off/all.hpp>
#include <id3d/common/all.hpp>

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

struct RenderSettings {
    bool show_edges = true;
    bool show_triangles = false;
    bool show_vertices = false;

    int selected_render_mode = 0;

    float line_width = 1.f;
    float point_size = 1.f;
};

struct App : Program {
    RenderSettings render_settings;

    eng::ShaderCompiler shader_compiler = {};

    eng::Camera camera;

    agl::engine::RenderPass line_pass;
    agl::engine::RenderPass point_pass;
    agl::engine::RenderPass triangle_pass;

    face_vertex::Mesh mesh;

    std::shared_ptr<agl::engine::MeshInstance> edges_mesh_i;
    std::shared_ptr<agl::engine::MeshInstance> triangle_connectivity_mesh_i;
    std::shared_ptr<agl::engine::MeshInstance> triangle_mesh_i;
    std::shared_ptr<agl::engine::MeshInstance> voronoi_mesh_i;

    Settings settings;

    void refresh_mesh() {
        edges_mesh_i = agl::standard::shared(agl::engine::instance(
            edges_mesh(mesh)));
        // triangle_connectivity_mesh_i = agl::standard::shared(agl::engine::instance(
        //     edges_mesh(mesh)));
    }

    void init() override {
        shader_compiler.log_folder = "logs/";
        shader_compiler.root = "chaine/src/";

        { // Render passes.
            line_pass.program = std::make_shared<eng::Program>(
                data::edge_program(shader_compiler));
            point_pass.program = std::make_shared<eng::Program>(
                data::vertex_program(shader_compiler));
            triangle_pass.program = std::make_shared<eng::Program>(
                data::triangle_program(shader_compiler));
        }

        mesh = face_vertex::Mesh();
        {
            auto v0 = create_vertex(mesh);
            position(v0) = paraboloid(-0.2f, -0.2f);
            auto v1 = create_vertex(mesh);
            position(v1) = paraboloid(0.2f, -0.2f);
            auto v2 = create_vertex(mesh);
            position(v2) = paraboloid(-0.2f, 0.2f);
            auto t = create_triangle(mesh);
            topology(t)->vertices[0] = v0;
            topology(t)->vertices[1] = v1;
            topology(t)->vertices[2] = v2;
            topology(v0)->triangle = t;
            topology(v1)->triangle = t;
            topology(v2)->triangle = t;
        }

        refresh_mesh();

        transit_to_2d();
    }

    bool mouse_state = false;

    void transit_to_2d() {
        settings.view_mode = ViewMode::_2D;
        { // Camera.
            auto op = agl::engine::OrthographicProjection();
            op.z_far = 10.f;
            op.z_near = -10.f;
            camera.projection = std::move(op);
            camera.view.roll = 0.f;
            camera.view.pitch = 0.f;
            camera.view.yaw = 0.f;
            camera.view.position[2] = 0.f;
        }
    }

    void update_2d() {
        if(not ImGui::GetIO().WantCaptureMouse) {
            { // Dragging.
                if(ImGui::IsMouseDown(0)) {
                    auto dx = 2.f * ImGui::GetIO().MouseDelta.x / (window.width() - 1);
                    auto dy = 2.f * ImGui::GetIO().MouseDelta.y / (window.height() - 1);
                    camera.view.position += agl::vec3(-dx, dy, 0.f);
                }
            }
            { // Insertion.
                if(ImGui::IsMouseClicked(1)) {
                    auto x = 2.f * ImGui::GetIO().MousePos.x / window.width() - 1.f;
                    auto y = -2.f * ImGui::GetIO().MousePos.y / window.height() + 1.f;
                    auto pos = (transform(camera) * agl::vec4(x, y, 0.f, 1.f)).xyz();
                    insert(mesh, pos.xy());
                    if(settings.delaunay_insertion) {
                        lawson(mesh);
                    }
                    refresh_mesh();
                }
            }
        }

    }

    void transit_to_3d() {
        settings.view_mode = ViewMode::_3D;
    }

    void update_3d() {
        if(not ImGui::GetIO().WantCaptureKeyboard) {
            { // Orientation.
                if(ImGui::IsMouseDown(0)) {
                    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    float dx = ImGui::GetIO().MouseDelta.x;
                    float dy = ImGui::GetIO().MouseDelta.y;
                    camera.view.yaw -= dx / 500.f;
                    camera.view.pitch -= dy / 500.f;
                } else {
                    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
            }
            { // Camera controls.

            }
        }
    }

    void update(float) override {
        update_2d();
        // {
        //     if(not mouse_state and glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_2)) {
        //         double xpos, ypos;
        //         glfwGetCursorPos(window.window, &xpos, &ypos);
        //         xpos = 2.f * xpos / window.width() - 1.f;
        //         ypos = -2.f * ypos / window.height() + 1.f;
                
        //         auto pos = (transform(camera) * agl::vec4(float(xpos), float(ypos), 0.f, 1.f)).xyz();

        //         insert(mesh, pos.xy());
        //         if(settings.delaunay_insertion) {
        //             lawson(mesh);
        //         }
        //         refresh_mesh();
        //     }
        // }
        // {
        //     if(glfwGetKey(window.window, GLFW_KEY_A)) {
        //         auto direction = (rotation(camera.view) * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
        //         camera.view.position = camera.view.position - direction / 50.f;
        //     }
        //     if(glfwGetKey(window.window, GLFW_KEY_D)) {
        //         auto direction = (rotation(camera.view) * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
        //         camera.view.position = camera.view.position + direction / 50.f;
        //     }
        //     if(glfwGetKey(window.window, GLFW_KEY_S)) {
        //         auto direction = (rotation(camera.view) * agl::rotation_x(agl::constant::pi / 2.f))[2].xyz();
        //         camera.view.position = camera.view.position + direction / 50.f;
        //     }
        //     if(glfwGetKey(window.window, GLFW_KEY_W)) {
        //         auto direction = (rotation(camera.view) * agl::rotation_x(agl::constant::pi / 2.f))[2].xyz();
        //         camera.view.position = camera.view.position - direction / 50.f;
        //     }
        // }
        // {
        //     mouse_state = glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_2);
        // }
    }

    void render() override {
        clear(agl::default_framebuffer, agl::depth_tag, 1.f);

        { // Line pass.
            clear(line_pass);
            line_pass.uniforms["mvp"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(
                agl::engine::world_to_clip(camera));

            glLineWidth(render_settings.line_width);
        }
        { // Point pass.
            clear(point_pass);
            point_pass.uniforms["mvp"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(
                agl::engine::world_to_clip(camera));
            point_pass.uniforms["point_size"]
            = std::make_shared<eng::Uniform<float>>(
                render_settings.point_size);
        }
        { // Triangle pass.
            clear(triangle_pass);
            triangle_pass.uniforms["mvp"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(
                agl::engine::world_to_clip(camera));
        }
        
        if(settings.show_edges) {
            subscribe(line_pass, edges_mesh_i);
            edges_mesh_i->uniforms["color"]
            = std::make_shared<eng::Uniform<agl::Vec3>>(
                agl::vec3(1.f, 0.f, 0.f));
        }
        if(settings.show_triangles) {
            // subscribe(triangle_pass, vertices_mesh_i);
        }
        if(settings.show_vertices) {
            // subscribe(point_pass, vertices_mesh_i);
        }

        agl::engine::render(triangle_pass);
        agl::engine::render(line_pass);
        agl::engine::render(point_pass);

        render_ui();
    }

    void render_ui();        
};
