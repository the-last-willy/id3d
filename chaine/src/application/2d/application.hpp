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

struct App : Program {
    eng::ShaderCompiler shader_compiler = {};

    eng::Camera camera;

    agl::engine::RenderPass line_pass;

    face_vertex::Mesh mesh;

    std::shared_ptr<agl::engine::MeshInstance> edges_mesh_i;
    std::shared_ptr<agl::engine::MeshInstance> previous_edges_mesh_i;
    std::shared_ptr<agl::engine::MeshInstance> triangle_connectivity_mesh_i;
    std::shared_ptr<agl::engine::MeshInstance> voronoi_mesh_i;

    Settings settings;

    void refresh_mesh() {
        edges_mesh_i = agl::standard::shared(agl::engine::instance(
            edges_mesh(mesh)));
        triangle_connectivity_mesh_i = agl::standard::shared(agl::engine::instance(
            triangle_adjacency_mesh(mesh)));
        voronoi_mesh_i = agl::standard::shared(agl::engine::instance(
            voronoi(mesh)));
    }

    void init() override {
        shader_compiler.log_folder = "logs/";
        shader_compiler.root = "chaine/src/";

        { // Render passes.
            line_pass.program = std::make_shared<eng::Program>(
                data::edge_program(shader_compiler));
        }

        mesh = face_vertex::Mesh();
        {
            auto v0 = create_vertex(mesh);
            position(v0) = paraboloid(-0.2f, -0.2f);
            auto v1 = create_vertex(mesh);
            position(v1) = paraboloid(0.2f, -0.2f);
            auto v2 = create_vertex(mesh);
            position(v2) = paraboloid(0.f, std::sqrt(3.f) / 5.f);
            auto t = create_triangle(mesh);
            topology(t)->vertices[0] = v0;
            topology(t)->vertices[1] = v1;
            topology(t)->vertices[2] = v2;
        }

        refresh_mesh();

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

    void update(float) override {
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

    void render() override {
        clear(agl::default_framebuffer, agl::depth_tag, 1.f);

        { // Line pass.
            clear(line_pass);
            line_pass.uniforms["mvp"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(
                agl::engine::world_to_clip(camera));

            glLineWidth(1.f);
        }
        if(settings.show_edges) {
            subscribe(line_pass, edges_mesh_i);
            edges_mesh_i->uniforms["color"]
            = std::make_shared<eng::Uniform<agl::Vec3>>(
                agl::vec3(1.f, 0.f, 0.f));
        }
        if(settings.show_triangle_connectivity) {
            subscribe(line_pass, triangle_connectivity_mesh_i);
            triangle_connectivity_mesh_i->uniforms["color"]
            = std::make_shared<eng::Uniform<agl::Vec3>>(
                agl::vec3(0.f, 0.f, 1.f));
        }
        if(settings.show_voronoi) {
            subscribe(line_pass, voronoi_mesh_i);
            voronoi_mesh_i->uniforms["color"]
            = std::make_shared<eng::Uniform<agl::Vec3>>(
                agl::vec3(0.f, 1.f, 0.f));
        }
        agl::engine::render(line_pass);

        render_ui();
    }

    void render_ui();        
};
