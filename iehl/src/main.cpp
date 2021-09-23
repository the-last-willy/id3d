// Disabled warnings.

#pragma warning(disable : 4005 4996)

// Local headers.

#include <agl/format/gltf2/all.hpp>
#include <agl/format/wavefront/all.hpp>
#include <agl/standard/all.hpp>

#include <local/all.hpp>
#include "data/all.hpp"
#include "program/all.hpp"

// External libraries.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <range/v3/view/indirect.hpp>
#include <range/v3/view/map.hpp>

// Standard library.

#include <iostream>

//

using namespace iehl;

struct DirectionalLight {
    agl::Vec3 direction = {};
    agl::Mat4 transform = {};

    std::shared_ptr<eng::Texture> shadow_map = std::make_shared<eng::Texture>();;
};

struct SpotLight {
    agl::Vec3 direction = {};
    agl::Mat4 transform = {};

    std::shared_ptr<eng::Texture> shadow_map = std::make_shared<eng::Texture>();;
};

struct PointLight {
    agl::Vec3 position = {};
};

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

    agl::engine::RenderPass ambient_pass;
    agl::engine::RenderPass blinn_phong_pass;
    
    float time = 0.f;

    void init() override {
        { // Shader compiler.
            shader_compiler.log_folder = "logs/";
            shader_compiler.root = "iehl/src/shader";
        }

        database = agl::format::wavefront::load(
            // "C:/Users/Willy/Desktop/data/bistro-small/exterior.obj",
            // "C:/Users/Willy/Desktop/data/bistro-small/");
            "C:/Users/yoanp/Documents/bistro-small/exterior.obj",
            "C:/Users/yoanp/Documents/bistro-small/");

        { // Render passes
            ambient_pass = data::wavefront::forward_ambient_render_pass(shader_compiler);
            blinn_phong_pass = data::wavefront::forward_blinn_phong_pass(shader_compiler);
        }
        { // Render passes subscriptions.
            for(auto& m : database.meshes) {
                subscribe(ambient_pass, m);
                subscribe(blinn_phong_pass, m);
            }
        }

        { // Camera.
            auto& c = *(camera = std::make_shared<eng::Camera>());
            if(auto pp = std::get_if<eng::PerspectiveProjection>(&c.projection)) {
                pp->aspect_ratio = 16.f / 9.f;
            }
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
        }

    }

    void render() override {
        clear(agl::default_framebuffer, agl::depth_tag, 1.f);

        auto vp_tr = agl::engine::view_to_camera_transform(*camera);

        auto normal_tr = agl::engine::normal_transform(*camera);

        auto light_position = (vp_tr * agl::vec4(2.f, 2.f, 2.f, 1.f)).xyz();
        auto view_position = (vp_tr * vec4(camera->view.position, 1.f)).xyz();

        for(auto& s : ambient_pass.subscriptions) {
            s.mesh->uniforms["mvp_transform"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(vp_tr);
        }
        if constexpr(true) { // Frustrum culling.
            auto frustrum = agl::engine::bounding_box(*camera);
            int count = 0;
            for(auto& s : ambient_pass.subscriptions) {
                auto bb = bounding_box(*s.mesh);
                if(are_intersecting(bb, frustrum)) {
                    count += 1;
                }
            }
            std::cout << "frustrum culling=" << count << std::endl;
        }
        if constexpr(true) { // Ambient pass.
            agl::engine::render(ambient_pass);
        }
        if constexpr(true) { // Blinn Phong pass.
            blinn_phong_pass.uniforms["light_position"]
            = std::make_shared<eng::Uniform<agl::Vec3>>(light_position);
            blinn_phong_pass.uniforms["normal_transform"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(normal_tr);
            blinn_phong_pass.uniforms["view_position"]
            = std::make_shared<eng::Uniform<agl::Vec3>>(view_position);
            agl::engine::render(blinn_phong_pass);
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
