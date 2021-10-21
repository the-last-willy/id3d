// Disabled warnings.

#pragma warning(disable : 4005 4996)

// Local headers.

#include <agl/format/gltf2/all.hpp>
#include <agl/format/wavefront/all.hpp>
#include <agl/standard/all.hpp>

#include <local/all.hpp>
#include "data/all.hpp"
#include "program/all.hpp"

#include "gizmo/gizmo.hpp"

// External libraries.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <range/v3/view/indirect.hpp>
#include <range/v3/view/map.hpp>

// Standard library.

#include <iostream>
#include <random>

//

// struct DirectionalLight {
//     agl::Vec3 direction = {};
//     agl::Mat4 transform = {};

//     std::shared_ptr<eng::Texture> shadow_map = std::make_shared<eng::Texture>();;
// };

// struct SpotLight {
//     agl::Vec3 direction = {};
//     agl::Mat4 transform = {};

//     std::shared_ptr<eng::Texture> shadow_map = std::make_shared<eng::Texture>();;
// };

// struct PointLight {
//     agl::Vec3 position = {};
// };

inline
auto create_random_generator() {
    auto rd = std::random_device();
    return std::default_random_engine(rd());
}

struct GltfProgram;

inline
void load_model(GltfProgram& program, const std::string& filepath);

struct GltfProgram : Program {
    eng::ShaderCompiler shader_compiler = {};

    // glTF file.
    agl::format::wavefront::Content database = {};

    // Default sampler.
    agl::Sampler default_sampler;

    // Ambient lighting.
    eng::Material ambient_light_mat = {};

    // Active camera.
    std::shared_ptr<eng::Camera> camera = {};

    bool ambient_pass_loaded = false;
    agl::engine::RenderPass ambient_pass;
    bool blinn_phong_pass_loaded = false;
    agl::engine::RenderPass blinn_phong_pass;

    agl::engine::RenderPass point_pass;

    // Gizmo gizmo;
    
    float time = 0.f;

    agl::engine::TriangleMesh inter_mesh;

    bool toggle_rasterization = true;
    
    std::default_random_engine random_generator = create_random_generator();

    void reload_shaders() {
        try {
            ambient_pass.program = std::make_shared<eng::Program>(
                data::wavefront::forward_ambient_program(shader_compiler));
            ambient_pass.subscriptions.clear();
            for(auto& m : database.meshes) {
                subscribe(ambient_pass, m);
            }
            ambient_pass_loaded = true;
        } catch(...) {
            ambient_pass.program.reset();
            std::cerr << "AMBIENT SHADERS FAILED TO COMPILE." << std::endl << std::endl;
            ambient_pass_loaded = false;
        }
        try {
            blinn_phong_pass.program = std::make_shared<eng::Program>(
                data::wavefront::forward_blinn_phong_program(shader_compiler));
                blinn_phong_pass.subscriptions.clear();
            for(auto& m : database.meshes) {
                subscribe(blinn_phong_pass, m);
            }
            blinn_phong_pass_loaded = true;
        } catch(...) {
            blinn_phong_pass.program.reset();
            std::cerr << "BLINN PHONG SHADERS FAILED TO COMPILE." << std::endl << std::endl;
            blinn_phong_pass_loaded = false;
        }
    }

    void reload_points() {
        auto render_inter_mesh = std::make_shared<eng::Mesh>(
            agl::engine::point_mesh(inter_mesh, database.materials));
        point_pass.subscriptions.clear();
        subscribe(point_pass, render_inter_mesh);
    }

    void init() override {
        { // Shader compiler.
            shader_compiler.log_folder = "logs/";
            shader_compiler.root = "iehl/src/shader";
        }
        
        database = agl::format::wavefront::load(
            "D:/data/cornell-box/cornell-box.obj",
            "D:/data/cornell-box");
            // "C:/Users/Willy/Desktop/data/wavefront/CornellBox/cornell-box.obj",
            // "C:/Users/Willy/Desktop/data/wavefront/CornellBox");
            // "D:/data/bistro-small/exterior.obj",
            // "D:/data/bistro-small/");
            // "C:/Users/Willy/Desktop/data/bistro-small/exterior.obj",
            // "C:/Users/Willy/Desktop/data/bistro-small/");
            // "C:/Users/yoanp/Documents/bistro-small/exterior.obj",
            // "C:/Users/yoanp/Documents/bistro-small/");

        // CLEAR GPU MESHES BECAUSE OF BADNESS ???
        // database.meshes.clear();
        // for(auto&& tm : database.tmeshes) {
        //     database.meshes.push_back(std::make_shared<eng::Mesh>(
        //         agl::engine::triangle_mesh(*tm, database.materials)));
        // }

        { // Normalize data.
            auto default_emissive = std::make_shared<eng::Texture>(
                data::uniform_texture(agl::vec3(0.f)));
            for(auto&& me : database.meshes | ranges::views::indirect) {
                for(auto&& p : me.primitives | ranges::views::indirect) {
                    if(p.material) {
                        auto&& ma = *p.material;
                        if(not ma.textures.contains("map_Ke")) {
                            ma.textures["map_Ke"] = default_emissive;
                        }
                    }
                }
            }
        }

        { // Render passes.
            reload_shaders();
        }

        { // Camera.
            auto& c = *(camera = std::make_shared<eng::Camera>());
            if(auto pp = std::get_if<eng::PerspectiveProjection>(&c.projection)) {
                pp->aspect_ratio = 16.f / 9.f;
            }
        }

        { // Point pass.
            point_pass.program = std::make_shared<eng::Program>(
                data::wavefront::point_program(shader_compiler));
        }
    }

    void update(float dt) override {
        { // Increment time.
            time += dt;
        }
        if(glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_1)) {
            glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            agl::Vec2 d = current_cursor_pos - previous_cursor_pos;
            camera->view.yaw -= d[0] / 500.f;
            camera->view.pitch -= d[1] / 500.f;

            previous_cursor_pos = current_cursor_pos;
        } else {
            glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            previous_cursor_pos = current_cursor_pos;
        }
        { // Camera controls.
            if(glfwGetKey(window.window, GLFW_KEY_A)) {
                auto direction = (rotation(camera->view)
                * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
                camera->view.position = camera->view.position - direction / 10.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_D)) {
                auto direction = (rotation(camera->view)
                * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
                camera->view.position = camera->view.position + direction / 10.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_S)) {
                auto direction = rotation(camera->view)[2].xyz();
                camera->view.position = camera->view.position + direction / 10.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_W)) {
                auto direction = rotation(camera->view)[2].xyz();
                camera->view.position = camera->view.position - direction / 10.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_R)) {
                reload_shaders();
            }
        }

        if(glfwGetKey(window.window, GLFW_KEY_SPACE)) {
            toggle_rasterization = not toggle_rasterization;
        }

        // rtx(); // on
        // reload_points();
    }

    

    void rtx() {
        auto& tmesh = *database.tmeshes.front();
        auto ro = camera->view.position;
        

        auto n = 300.f;
        for(float i = 0; i < n; ++i) {    
            auto radius = i / 200.f + std::uniform_real_distribution<float>(0.f, agl::constant::pi / 100.f)(random_generator);
            auto angle = i + std::uniform_real_distribution<float>(0.f, 1.f)(random_generator);
            auto dir = normalize(agl::vec3(
                radius * std::cos(angle), radius * std::sin(angle), -1.f));
            auto rd = (rotation(camera->view) * agl::vec4(dir, 0.f)).xyz();
            auto r = agl::engine::Ray(ro, rd);

            float ray_t = 1000.f;
            auto pos = agl::vec3(0.f);
            uint32_t fi = 0;

            uint32_t t = 0;
            for(; t < face_count(tmesh); ++t) {
                auto tr = face(tmesh, t);
                auto h = intersection(r, tr);
                if(h) {
                    pos = h->position;
                    ray_t = h->ray;
                    fi = t;
                    break;
                }
            }
            for(++t; t < face_count(tmesh); ++t) {
                auto tr = face(tmesh, t);
                auto h = intersection(r, tr);
                if(h and h->ray < ray_t) {
                    pos = h->position;
                    ray_t = h->ray;
                    fi = t;
                }
            }
            if(ray_t < 1000.f) {
                auto v = create_vertex(inter_mesh);
                position(v) = pos;
                auto ivert = incident_vertex(face(tmesh, fi), 0);
                normal(v) = normal(ivert);
                auto mid = material_id(ivert);
                auto& m = database.materials[mid];
                if(auto ptr = dynamic_cast<eng::Uniform<agl::Vec3>*>(m->uniforms["Kd"])) {
                    color(v) = ptr->value;
                } else {
                    std::cout << "no color" << std::endl;
                }
            }
        }
    }

    void render() override {
        clear(agl::default_framebuffer, agl::depth_tag, 1.f);

        auto vp_tr = agl::engine::world_to_clip(*camera);
        auto v_tr = agl::engine::world_to_eye(*camera);
        auto normal_tr = agl::engine::normal_transform(*camera);

        auto light_position = (agl::vec4(0.f, 0.f, 0.f, 1.f)).xyz();
        auto view_position = (vec4(camera->view.position, 1.f)).xyz();

        if constexpr(true) {
            for(auto& s : ambient_pass.subscriptions) {
                s.mesh->uniforms["mvp_transform"]
                = std::make_shared<eng::Uniform<agl::Mat4>>(vp_tr);
            }
        }
        if constexpr(false) {
            for(auto& s : blinn_phong_pass.subscriptions) {
                s.mesh->uniforms["mvp_transform"]
                = std::make_shared<eng::Uniform<agl::Mat4>>(vp_tr);
            }
        }
        if constexpr(false) { // Frustrum culling.
            auto frustrum = agl::engine::bounding_box(*camera);
            
            int count = 0;
            for(auto& s : ambient_pass.subscriptions) {
                auto bb = bounding_box(*s.mesh);
                s.mesh->enabled = false;
                if(are_intersecting(bb, frustrum)) {
                    s.mesh->enabled = true;
                    count += 1;
                }
            }
        }
        if(toggle_rasterization) {
            if(ambient_pass_loaded) { // Ambient pass.
                agl::engine::render(ambient_pass);
            }
        }
        if constexpr(false) {
            if(blinn_phong_pass_loaded) { // Blinn Phong pass.
                blinn_phong_pass.uniforms["light_position"]
                = std::make_shared<eng::Uniform<agl::Vec3>>(light_position);
                blinn_phong_pass.uniforms["normal_transform"]
                = std::make_shared<eng::Uniform<agl::Mat4>>(normal_tr);
                blinn_phong_pass.uniforms["view_position"]
                = std::make_shared<eng::Uniform<agl::Vec3>>(view_position);
                agl::engine::render(blinn_phong_pass);
            }
        }

        if constexpr(true) { // Point pass.
            point_pass.uniforms["mvp_transform"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(vp_tr);
            point_pass.uniforms["world_to_eye"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(v_tr);
            agl::engine::render(point_pass);
        }
    }
};

void throwing_main() {
    auto p = GltfProgram();
    run(p);
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
